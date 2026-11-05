#Requires -Version 5.1
<#
.SYNOPSIS
    Runs the end-to-end autotest: the real game, unattended, in a scratch folder.

.DESCRIPTION
    Copies the built game into build\autotest (the two big data files are hard linked,
    not copied), with The Heaven's shipped res\config.ini, then runs the game once per
    phase with "-autotest <phase>". Each run creates or loads its own test hero, runs its
    checks and quits; see src\AutoTest.cpp for what each phase checks. Your own saves,
    shared stash and settings are never touched. The oldsave phase runs first, on the
    v0.2.0 save in tests\saves\v0.2.0, and its save folders are deleted after it.

    The game window opens off-screen, without focus and muted, so you can keep using the
    PC. -Show puts it on screen to watch.

    Results: build\autotest\autotest-report.json (all checks), autotest-*.gif screenshots,
    and a summary printed at the end. Exit code 0 when every check passed.

.PARAMETER Build
    Build Release first.

.PARAMETER Show
    Show the game window on screen (still muted, still doesn't take focus).

.PARAMETER Only
    Run only these phases. other and owner need the shared stash main leaves behind and
    every run starts from a fresh folder, so on their own they fail: use it to run main or
    oldsave alone.

.PARAMETER TimeoutSec
    How long one phase may run before it counts as hung and is stopped.

.EXAMPLE
    .\scripts\autotest.ps1
    .\scripts\autotest.ps1 -Build -Show
#>
param(
    [switch]$Build,
    [switch]$Show,
    [ValidateSet("oldsave", "main", "other", "owner")][string[]]$Only,
    [int]$TimeoutSec = 240
)

Set-StrictMode -Version Latest
$ErrorActionPreference = "Stop"

$repo  = Resolve-Path "$PSScriptRoot\.."
$buildDir = Join-Path $repo "build"
$run   = Join-Path $buildDir "autotest"
$phases = @("oldsave", "main", "other", "owner") # in this order: other and owner use what main left
if ($Only) { $phases = @($phases | Where-Object { $Only -contains $_ }) }

function Fail([string]$msg) { Write-Host "ERROR: $msg" -ForegroundColor Red; exit 2 }

# Stops every game started from the scratch folder (and only those: your own game runs from
# elsewhere), and waits until they are gone so their files can be deleted.
function Stop-AutotestGames {
    $prefix = (Join-Path $run "") # trailing backslash
    for ($i = 0; $i -lt 50; $i++) {
        $left = @(Get-Process -Name TheHeaven -ErrorAction SilentlyContinue |
            Where-Object { $_.Path -and $_.Path.StartsWith($prefix, [StringComparison]::OrdinalIgnoreCase) })
        if ($left.Count -eq 0) { return }
        $left | Stop-Process -Force -ErrorAction SilentlyContinue
        Start-Sleep -Milliseconds 200
    }
    Fail "could not stop the autotest game still running from $run"
}

if ($Build) {
    & "$PSScriptRoot\build.ps1" Release
    if ($LASTEXITCODE -ne 0) { Fail "build failed" }
}

foreach ($f in @("TheHeaven.exe", "diabdat.mpq", "TH4data.mor")) {
    if (-not (Test-Path (Join-Path $buildDir $f))) { Fail "build\$f not found (build first, and copy the game data, see docs\BUILD.md)" }
}

# ---- fresh scratch copy of the game -----------------------------------------
Stop-AutotestGames
if (Test-Path $run) { Remove-Item $run -Recurse -Force }
New-Item -ItemType Directory $run | Out-Null

foreach ($f in @("diabdat.mpq", "TH4data.mor")) {
    $src = Join-Path $buildDir $f
    $dst = Join-Path $run $f
    try { New-Item -ItemType HardLink -Path $dst -Target $src -ErrorAction Stop | Out-Null }
    catch { Copy-Item $src $dst } # another drive: copy instead
}
foreach ($f in @("TheHeaven.exe", "TheHeaven.pdb", "Strm2.dll", "SmackW32.DLL", "Standard.sn2")) {
    $src = Join-Path $buildDir $f
    if (Test-Path $src) { Copy-Item $src $run }
}
foreach ($d in @("Data", "ui_art")) {
    $src = Join-Path $buildDir $d
    if (Test-Path $src) { Copy-Item $src (Join-Path $run $d) -Recurse }
}
Copy-Item (Join-Path $repo "res\config.ini") (Join-Path $run "config.ini") # what players get

# ---- run the phases -----------------------------------------------------------
$results = @()
$oldSave = Join-Path $repo "tests\saves\v0.2.0"
foreach ($phase in $phases) {
    Write-Host "==> autotest $phase" -ForegroundColor Cyan
    if ($phase -eq "oldsave") { Copy-Item (Join-Path $oldSave "*") $run -Recurse -Exclude "README.md" }
    $gameArgs = @("-autotest", $phase)
    if ($Show) { $gameArgs += "-show" }
    $p = Start-Process -FilePath (Join-Path $run "TheHeaven.exe") -ArgumentList $gameArgs -WorkingDirectory $run -PassThru
    try { $p.PriorityClass = "BelowNormal" } catch {}
    # The game logs "config read" once it has switched itself to the quiet off-screen
    # settings; if it doesn't, it would run as a normal full-screen game with sound, so stop it.
    $logFile = Join-Path $run "autotest-$phase.log"
    $quiet = $false
    for ($i = 0; $i -lt 150 -and -not $p.HasExited; $i++) {
        if ((Test-Path $logFile) -and (Select-String -Path $logFile -Pattern "config read" -SimpleMatch -Quiet)) { $quiet = $true; break }
        Start-Sleep -Milliseconds 200
    }
    if (-not $quiet -and -not $p.HasExited) {
        Stop-AutotestGames
        Fail "phase ${phase}: the game didn't switch to the autotest settings within 30 s, stopped it"
    }
    $exited = $p.WaitForExit($TimeoutSec * 1000)
    if (-not $exited) {
        Stop-Process -Id $p.Id -Force
        Write-Host "    timed out after $TimeoutSec s, stopped" -ForegroundColor Red
    }
    Stop-AutotestGames # nothing may outlive its phase
    $reportFile = Join-Path $run "autotest-$phase.json"
    if (Test-Path $reportFile) {
        $report = Get-Content $reportFile -Raw | ConvertFrom-Json
    } else {
        $why = if (-not $exited) { "timed out" } else { "the game quit without a report (exit code $($p.ExitCode)); look for crash logs in build\autotest" }
        $report = [pscustomobject]@{ phase = $phase; passed = 0; failed = 1; screenshots = @()
            checks = @([pscustomobject]@{ name = "phase $phase runs to the end"; pass = $false; detail = $why }) }
    }
    foreach ($c in $report.checks) {
        $mark  = if ($c.pass) { "PASS" } else { "FAIL" }
        $color = if ($c.pass) { "Green" } else { "Red" }
        Write-Host ("    {0}  {1}" -f $mark, $c.name) -ForegroundColor $color
        if ($c.detail -and -not $c.pass) { Write-Host "          $($c.detail)" -ForegroundColor DarkGray }
    }
    $results += $report
    if ($phase -eq "oldsave") { # the next phases start from no saves
        Get-ChildItem $run -Directory -Filter "th4_save_*" | Remove-Item -Recurse -Force
        Remove-Item (Join-Path $run "oldsave-manifest.txt") -ErrorAction SilentlyContinue
        continue
    }
    if ($report.failed -gt 0 -and -not (Test-Path $reportFile)) { break } # later phases need this one
}

# ---- summary --------------------------------------------------------------------
$passed = ($results | Measure-Object -Property passed -Sum).Sum
$failed = ($results | Measure-Object -Property failed -Sum).Sum
$summary = [pscustomobject]@{
    date   = (Get-Date).ToString("s")
    commit = (git -C $repo rev-parse --short HEAD)
    passed = $passed
    failed = $failed
    phases = $results
}
$summary | ConvertTo-Json -Depth 6 | Set-Content (Join-Path $run "autotest-report.json") -Encoding UTF8

Write-Host ""
$color = if ($failed -eq 0) { "Green" } else { "Red" }
Write-Host "$passed passed, $failed failed. Report and screenshots: build\autotest" -ForegroundColor $color
if ($failed -gt 0) { exit 1 }
exit 0
