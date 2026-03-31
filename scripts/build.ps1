$msbuild = "C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\MSBuild\Current\Bin\MSBuild.exe"
$config = if ($args.Count -gt 0) { $args[0] } else { "Debug" }

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

& $msbuild TheHeaven.sln /p:Configuration=$config /p:Platform=Win32

# Copy PDB next to the exe so crash logs can resolve symbols
if (Test-Path "$config\TheHeaven.pdb") {
    Copy-Item "$config\TheHeaven.pdb" "build\TheHeaven.pdb" -Force
}

# Clean up intermediate build artifacts
if (Test-Path "Release") {
    Remove-Item -Recurse -Force "Release"
}
