$msbuild = "C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\MSBuild\Current\Bin\MSBuild.exe"
$config = if ($args[0]) { $args[0] } else { "Release" }

& $msbuild TheHell_4.sln /p:Configuration=$config /p:Platform=Win32
