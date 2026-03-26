#Requires -Version 5.1
<#
.SYNOPSIS
    Builds and publishes a GitHub release for Diablo: The Heaven.

.DESCRIPTION
    Reads version from tools/version.h, extracts release notes from docs/CHANGELOG.md,
    builds the project, and creates a GitHub release with all files from the build folder.

.PARAMETER Version
    Optional version override (e.g. "0.0.1"). If not provided, reads from tools/version.h.

.PARAMETER DryRun
    If set, performs all steps except actually creating the GitHub release.

.EXAMPLE
    .\scripts\release.ps1
    .\scripts\release.ps1 -DryRun
    .\scripts\release.ps1 -Version "0.0.2"
#>
param(
    [string]$Version,
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
    $match = [regex]::Match($header, 'THE_HELL_VERSION_STRING\s+"v([^\\]+)\\0"')
    if (-not $match.Success) { Fail "Could not parse version from tools/version.h" }
    $Version = $match.Groups[1].Value
}

$tag = "v$Version"
Write-Ok "Version: $tag"

# ── Safety checks ────────────────────────────────────────────────────────────

Write-Step "Running pre-release checks"

# Check CHEATS is disabled
$header = Get-Content "tools/version.h" -Raw
if ($header -match '#define CHEATS\s+1') {
    Fail "CHEATS is enabled in tools/version.h — set it to 0 before releasing"
}
Write-Ok "CHEATS is disabled"

# Check gh CLI is available
if (-not (Get-Command gh -ErrorAction SilentlyContinue)) {
    Fail "GitHub CLI (gh) not found. Install from https://cli.github.com/"
}
Write-Ok "GitHub CLI found"

# Check tag doesn't already exist
$existingTag = gh release view $tag 2>&1
if ($LASTEXITCODE -eq 0) {
    Fail "Release $tag already exists on GitHub. Bump the version first."
}
Write-Ok "Tag $tag is available"

# ── Extract changelog ────────────────────────────────────────────────────────

Write-Step "Extracting release notes from docs/CHANGELOG.md"

$changelog = Get-Content "docs/CHANGELOG.md" -Raw
$match = [regex]::Match($changelog, "## \[$Version\].*?\n([\s\S]*?)(?=\n## \[|\z)")
if (-not $match.Success) {
    Write-Warn "No changelog entry found for [$Version] — release notes will be empty"
    $releaseNotes = "_No release notes provided._"
} else {
    $releaseNotes = $match.Groups[1].Value.Trim()
    Write-Ok "Found release notes ($($releaseNotes.Length) chars)"
}

# ── Build ────────────────────────────────────────────────────────────────────

Write-Step "Building release"

& ".\build.ps1"
if ($LASTEXITCODE -ne 0) { Fail "Build failed — aborting release" }
Write-Ok "Build succeeded"

# ── Collect build artifacts ──────────────────────────────────────────────────

Write-Step "Collecting build artifacts"

if (-not (Test-Path "build")) { Fail "build/ folder not found after build step" }

$buildFiles = Get-ChildItem "build" -Recurse -File | ForEach-Object { $_.FullName }
if ($buildFiles.Count -eq 0) { Fail "build/ folder is empty — nothing to release" }

Write-Ok "Files to upload:"
$buildFiles | ForEach-Object { Write-Host "    $_" }

# ── Create release ───────────────────────────────────────────────────────────

Write-Step "Creating GitHub release $tag"

if ($DryRun) {
    Write-Warn "DryRun mode — skipping gh release create"
    Write-Host "`nWould run:" -ForegroundColor DarkGray
    Write-Host "  gh release create `"$tag`" <$($buildFiles.Count) files> -t `"$tag`" -n `"...`"" -ForegroundColor DarkGray
} else {
    gh release create $tag @buildFiles --title $tag --notes $releaseNotes
    if ($LASTEXITCODE -ne 0) { Fail "gh release create failed" }
    Write-Ok "Release published: https://github.com/$(gh repo view --json nameWithOwner -q .nameWithOwner)/releases/tag/$tag"
}

Write-Host "`nDone!" -ForegroundColor Green
