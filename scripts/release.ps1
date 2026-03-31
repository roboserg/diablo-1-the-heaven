#Requires -Version 5.1
<#
.SYNOPSIS
    Builds and publishes a GitHub release for Diablo: The Heaven.

.DESCRIPTION
    Reads version from tools/version.h, extracts release notes from docs/CHANGELOG.md,
    builds the project, packages release files into a zip, and creates a GitHub release.

.PARAMETER Version
    Optional version override (e.g. "0.0.1"). If not provided, reads from tools/version.h.

.PARAMETER TagPrefix
    Optional tag prefix override (e.g. "experimental-"). Applied before the version tag.

.PARAMETER TagSuffix
    Optional tag suffix override (e.g. "mp8-test1"). Appended after the version tag as "-suffix".

.PARAMETER Prerelease
    If set, creates the GitHub release as a prerelease.

.PARAMETER DryRun
    If set, performs all steps except actually creating the GitHub release.

.EXAMPLE
    .\scripts\release.ps1
    .\scripts\release.ps1 -DryRun
    .\scripts\release.ps1 -Version "0.0.2"
    .\scripts\release.ps1 -TagPrefix "experimental-" -TagSuffix "mp8-test1"
#>
param(
    [string]$Version,
    [string]$TagPrefix = "",
    [string]$TagSuffix = "",
    [switch]$Prerelease,
    [switch]$DryRun
)

Set-StrictMode -Version Latest
$ErrorActionPreference = "Stop"

# ── Helpers ─────────────────────────────────────────────────────────────────

function Write-Step([string]$msg) { Write-Host "`n==> $msg" -ForegroundColor Cyan }
function Write-Ok([string]$msg)   { Write-Host "    $msg" -ForegroundColor Green }
function Write-Warn([string]$msg) { Write-Host "    WARNING: $msg" -ForegroundColor Yellow }
function Fail([string]$msg)       { Write-Host "`nERROR: $msg" -ForegroundColor Red; exit 1 }

# ── Resolve version ──────────────────────────────────────────────────────────

Write-Step "Resolving version"

if (-not $Version) {
    $header = Get-Content "tools/version.h" -Raw
    $match = [regex]::Match($header, 'THE_HELL_VERSION_STRING\s+"v([^"]+)"')
    if (-not $match.Success) { Fail "Could not parse version from tools/version.h" }
    $Version = $match.Groups[1].Value
}

$tag = "${TagPrefix}v$Version"
if ($TagSuffix) {
    $tag = "$tag-$TagSuffix"
}
Write-Ok "Version: $tag"

# ── Safety checks ────────────────────────────────────────────────────────────

Write-Step "Running pre-release checks"

# Check CHEATS is disabled
$header = Get-Content "tools/version.h" -Raw
if ($header -notmatch '#ifdef _DEBUG' -and $header -match '#define CHEATS\s+1') {
    Fail "CHEATS is hardcoded to 1 in tools/version.h — use the #ifdef _DEBUG guard instead"
}
Write-Ok "CHEATS is auto-managed via #ifdef _DEBUG"

# Check gh CLI is available
if (-not (Get-Command gh -ErrorAction SilentlyContinue)) {
    Fail "GitHub CLI (gh) not found. Install from https://cli.github.com/"
}
Write-Ok "GitHub CLI found"

# Check tag doesn't already exist
gh release view $tag | Out-Null
if ($LASTEXITCODE -eq 0) {
    Fail "Release $tag already exists on GitHub. Bump the version first."
}
Write-Ok "Tag $tag is available"

# ── Extract changelog ────────────────────────────────────────────────────────

Write-Step "Extracting release notes from docs/CHANGELOG.md"

$changelog = Get-Content "docs/CHANGELOG.md" -Raw -Encoding UTF8
$pattern = '## \[' + $Version + '\].*?\n([\s\S]*?)(?=\n## \[|\z)'
$match = [regex]::Match($changelog, $pattern)
if (-not $match.Success) {
    Write-Warn "No changelog entry found for [$Version] — release notes will be empty"
    $releaseNotes = "_No release notes provided._"
} else {
    $releaseNotes = $match.Groups[1].Value.Trim()
    Write-Ok "Found release notes ($($releaseNotes.Length) chars)"
}

# ── Build ────────────────────────────────────────────────────────────────────

Write-Step "Building release"

& "$PSScriptRoot\build.ps1" Release
if ($LASTEXITCODE -ne 0) { Fail "Build failed — aborting release" }
Write-Ok "Build succeeded"

# ── Package release zip ──────────────────────────────────────────────────────

Write-Step "Packaging release zip"

if (-not (Test-Path "build")) { Fail "build/ folder not found after build step" }

# Files to include in the release zip
# Standard.sn2 is required for multiplayer (Battle.net-style networking)
$releaseFiles = @(
    "TheHeaven.exe",
    "Strm2.dll",
    "SmackW32.DLL",
    "config.ini",
    "Standard.sn2"
)

$zipName = "DiabloTheHeaven-$tag.zip"
$zipPath = "$PSScriptRoot\..\$zipName"

# Verify all required files/folders exist
foreach ($f in $releaseFiles) {
    $fullPath = "build\$f"
    if (-not (Test-Path $fullPath)) {
        Write-Warn "Expected release file not found, skipping: $fullPath"
    }
}

# Build the zip
if (Test-Path $zipPath) { Remove-Item $zipPath -Force }

Add-Type -AssemblyName System.IO.Compression.FileSystem
$zip = [System.IO.Compression.ZipFile]::Open($zipPath, 'Create')

foreach ($f in $releaseFiles) {
    $fullPath = Resolve-Path "build\$f" -ErrorAction SilentlyContinue
    if (-not $fullPath) { continue }

    if (Test-Path $fullPath -PathType Container) {
        # Add folder contents recursively
        Get-ChildItem $fullPath -Recurse -File | ForEach-Object {
            $entryName = $_.FullName.Substring((Resolve-Path "build").Path.Length + 1)
            [System.IO.Compression.ZipFileExtensions]::CreateEntryFromFile($zip, $_.FullName, $entryName) | Out-Null
        }
    } else {
        [System.IO.Compression.ZipFileExtensions]::CreateEntryFromFile($zip, $fullPath, $f) | Out-Null
    }
}

$zip.Dispose()

$zipSize = [math]::Round((Get-Item $zipPath).Length / 1MB, 2)
Write-Ok "Created $zipName ($zipSize MB)"

# ── Create release ───────────────────────────────────────────────────────────

Write-Step "Creating GitHub release $tag"

$releaseArgs = @($tag, $zipPath, "--title", $tag, "--notes-file", "")
if ($Prerelease) {
    Write-Ok "Release type: prerelease"
} else {
    Write-Ok "Release type: stable"
}

if ($DryRun) {
    Write-Warn "DryRun mode — skipping gh release create"
    Write-Host "`nWould run:" -ForegroundColor DarkGray
    $dryRunSuffix = if ($Prerelease) { " --prerelease" } else { "" }
    Write-Host "  gh release create `"$tag`" `"$zipPath`" --title `"$tag`" --notes `"...`"$dryRunSuffix" -ForegroundColor DarkGray
} else {
    $notesFile = [System.IO.Path]::GetTempFileName()
    [System.IO.File]::WriteAllText($notesFile, $releaseNotes, [System.Text.Encoding]::UTF8)
    try {
        $releaseArgs[5] = $notesFile
        if ($Prerelease) {
            $releaseArgs += "--prerelease"
        }
        gh release create @releaseArgs
        if ($LASTEXITCODE -ne 0) { Fail "gh release create failed" }
        Write-Ok "Release published: https://github.com/$(gh repo view --json nameWithOwner -q .nameWithOwner)/releases/tag/$tag"
        Remove-Item $zipPath -Force
        Write-Ok "Removed local $zipName"
    } finally {
        Remove-Item $notesFile -ErrorAction SilentlyContinue
    }
}

Write-Host ""
Write-Host "Done!" -ForegroundColor Green
