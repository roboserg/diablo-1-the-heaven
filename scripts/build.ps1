$config = if ($args.Count -gt 0) { $args[0] } else { "Debug" }

# Use the newest Visual Studio / Build Tools installed. The project targets v145
# (VS 2026); with VS 2022 it builds with the v143 toolset instead.
$vswhere = "${env:ProgramFiles(x86)}\Microsoft Visual Studio\Installer\vswhere.exe"
if (-not (Test-Path $vswhere)) { throw "Visual Studio Build Tools not found (no vswhere.exe). See docs/BUILD.md." }
$vs = & $vswhere -latest -products * -requires Microsoft.Component.MSBuild -format json | ConvertFrom-Json | Select-Object -First 1
if (-not $vs) { throw "No Visual Studio installation with MSBuild found. See docs/BUILD.md." }
$msbuild = Join-Path $vs.installationPath "MSBuild\Current\Bin\MSBuild.exe"
$toolset = @()
if ([int]$vs.installationVersion.Split('.')[0] -lt 18) { $toolset += "/p:PlatformToolset=v143" }
Write-Host "Using $($vs.displayName) $($vs.installationVersion) $toolset"

# Stop this workspace's running game instances so the linker can overwrite build\TheHeaven.exe.
$targetExe = [System.IO.Path]::GetFullPath((Join-Path (Get-Location) "build\TheHeaven.exe"))
Get-Process -Name "TheHeaven" -ErrorAction SilentlyContinue | ForEach-Object {
    try {
        if ($_.Path -and [System.IO.Path]::GetFullPath($_.Path) -eq $targetExe) {
            Write-Host "Stopping running instance locking output: $($_.Id)" -ForegroundColor Yellow
            Stop-Process -Id $_.Id -Force
        }
    } catch {
        # Ignore processes we cannot inspect.
    }
}

& $msbuild TheHeaven.sln /p:Configuration=$config /p:Platform=Win32 /m @toolset
$buildExit = $LASTEXITCODE

# Copy PDB next to the exe so crash logs can resolve symbols
if (Test-Path "$config\TheHeaven.pdb") {
    Copy-Item "$config\TheHeaven.pdb" "build\TheHeaven.pdb" -Force
}

# Clean up intermediate build artifacts
if (Test-Path "Release") {
    Remove-Item -Recurse -Force "Release"
}

exit $buildExit
