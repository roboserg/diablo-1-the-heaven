$msbuild = "C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\MSBuild\Current\Bin\MSBuild.exe"
$config = if ($args.Count -gt 0) { $args[0] } else { "Debug" }

& $msbuild TheHeaven.sln /p:Configuration=$config /p:Platform=Win32

# Clean up intermediate build artifacts
if (Test-Path "Release") {
    Remove-Item -Recurse -Force "Release"
}

