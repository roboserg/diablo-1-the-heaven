# Building The Hell 4 on Windows 11

## Prerequisites

Install **Visual Studio Build Tools** (2022 or 2026) with the following components:

| Component | Where to find it |
|---|---|
| Desktop development with C++ workload | Workloads tab |
| MSVC Build Tools for x64/x86 (Latest) | Included automatically |
| Windows 11 SDK (any recent version, e.g. 10.0.26100.0) | Optional → Desktop development with C++ |

The project targets the **v145** toolset (Build Tools 2026). With Build Tools 2022 you build with **v143** by passing `/p:PlatformToolset=v143` (see step 3). No XP support, no v141, no CMake/vcpkg/testing tools needed.

---

## Steps

### 1. Run InitProject.bat

From the repo root in a terminal:

```
InitProject.bat
```

This copies `tools\personal.props` to `personal.props` (your local build settings, ignored by git) if it doesn't exist yet. It also tries to copy a `thehell4.vcxproj.user` debugger settings file that isn't part of this release; the "file not found" message for it is harmless.

### 2. Edit personal.props

Open `personal.props` and set:

```xml
<GameFolder>$(MSBuildProjectDirectory)\build\</GameFolder>
<FxCopDir>C:\Program Files (x86)\Windows Kits\10\bin\10.0.26100.0\x86</FxCopDir>
```

- **GameFolder** — where the built `TH4.exe` will be placed. Must end with a backslash `\`. `$(MSBuildProjectDirectory)\build\` outputs into a `build/` folder at the repo root (ignored by git).
- **FxCopDir** — directory containing `fxc.exe` (the HLSL shader compiler). Use the version folder of the Windows SDK you installed (look under `C:\Program Files (x86)\Windows Kits\10\bin\`).

### 3. Build

#### Option A — From VS Code (recommended)

Press `Ctrl+Shift+B` — VS Code will invoke MSBuild using the project's `.sln` file.

> If no build task exists yet, open the Command Palette (`Ctrl+Shift+P`) → **Tasks: Configure Default Build Task** → **Create tasks.json from template** → **MSBuild**, then point it at `TheHell_4.sln`.

#### Option B — From PowerShell

Build Tools 2026 (v145):

```
& "C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\MSBuild\Current\Bin\MSBuild.exe" TheHell_4.sln /p:Configuration=Release /p:Platform=Win32 /m
```

Build Tools 2022 (v143):

```
& "C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\MSBuild\Current\Bin\MSBuild.exe" TheHell_4.sln /p:Configuration=Release /p:Platform=Win32 /p:PlatformToolset=v143 /m
```

Or simply use the provided script, which picks whichever Build Tools you have:

```
.\build.ps1           # Release
.\build.ps1 Debug     # Debug
```

---

## After Building

Copy these files into your `GameFolder` to run the game:

```
diabdat.mpq     from Diablo 1 (required)
TH4data.mor     from The Hell 4 v0.774 (required: graphics and sounds)
THmusic.mor     from The Hell 4 (optional music)
THmusic2.mor    from The Hell 4 (optional music)
THmusic3.mor    from The Hell 4 (optional music)
```

`config.ini` is created on first launch. Then launch `TH4.exe` directly or via the VS debugger.

---

## Troubleshooting

| Error | Fix |
|---|---|
| `Cannot find fxc.exe` | Check `FxCopDir` in `personal.props` points to a folder that contains `fxc.exe` |
| `personal.props not found` | Run `InitProject.bat` first |
| `GameFolder` errors | Make sure the path ends with a backslash `\` and the folder exists |
| `v145 toolset not found` | You have Build Tools 2022: build with `/p:PlatformToolset=v143`, or use `.\build.ps1` |
| `RC2135: file not found: ../res/credits.txt` | The v0.774 release omitted `res/credits.txt` and `res/support.txt`; this fork restores them from v0.208 |
