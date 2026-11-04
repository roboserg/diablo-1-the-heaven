# Builds TheHell_4.sln with the newest Visual Studio / Build Tools installed.
# The project targets v145 (VS 2026); with VS 2022 it builds with v143 instead.
$config = if ($args[0]) { $args[0] } else { "Release" }

$vswhere = "${env:ProgramFiles(x86)}\Microsoft Visual Studio\Installer\vswhere.exe"
if (-not (Test-Path $vswhere)) { throw "Visual Studio Build Tools not found (no vswhere.exe). See BUILD.md." }

$vs = & $vswhere -latest -products * -requires Microsoft.Component.MSBuild -format json | ConvertFrom-Json | Select-Object -First 1
if (-not $vs) { throw "No Visual Studio installation with MSBuild found. See BUILD.md." }

$msbuild = Join-Path $vs.installationPath "MSBuild\Current\Bin\MSBuild.exe"
$extra = @()
if ([int]$vs.installationVersion.Split('.')[0] -lt 18) { $extra += "/p:PlatformToolset=v143" }

Write-Host "Using $($vs.displayName) $($vs.installationVersion) $extra"
& $msbuild TheHell_4.sln /p:Configuration=$config /p:Platform=Win32 /m @extra
exit $LASTEXITCODE
