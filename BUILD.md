# Building The Hell 4 on Windows 11

## Prerequisites

Install **Visual Studio Build Tools** with the following components:

| Component | Where to find it |
|---|---|
| Desktop development with C++ workload | Workloads tab |
| MSVC Build Tools for x64/x86 (Latest) | Included automatically |
| Windows 11 SDK (10.0.22621.0) | Optional → Desktop development with C++ |

No XP support, no v141, no CMake/vcpkg/testing tools needed.

---

## Steps

### 1. Run InitProject.bat

From the repo root in a terminal:

```
InitProject.bat
```

This creates two files if they don't already exist:
- `personal.props` — your local build settings
- `thehell4.vcxproj.user` — debugger settings

### 2. Edit personal.props

Open `personal.props` and set:

```xml
<GameFolder>C:\path\to\your\output\dir\</GameFolder>
<FxCopDir>C:\Program Files (x86)\Windows Kits\10\bin\10.0.22621.0\x86</FxCopDir>

```

- **GameFolder** — where the built `TH4.exe` will be placed. Must end with a backslash `\`. The default `$(MSBuildProjectDirectory)\build\` outputs into a `build/` folder at the repo root — no changes needed unless you want a different location.
- **FxCopDir** — directory containing `fxc.exe` (the HLSL shader compiler). The path above is correct if you installed Windows 11 SDK 10.0.22621.0.

### 3. Build

#### Option A — From VS Code (recommended)

Press `Ctrl+Shift+B` — VS Code will invoke MSBuild using the project's `.sln` file.

> If no build task exists yet, open the Command Palette (`Ctrl+Shift+P`) → **Tasks: Configure Default Build Task** → **Create tasks.json from template** → **MSBuild**, then point it at `TheHell_4.sln`.

#### Option B — From a Developer Command Prompt

Open a **Developer Command Prompt for VS Build Tools**, navigate to the repo root and run:

```
& "C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\MSBuild\Current\Bin\MSBuild.exe" TheHell_4.sln /p:Configuration=Release /p:Platform=Win32
```

Or simply use the provided script:
```
.\build.ps1
```

Use `Configuration=Debug` for a debug build.

---

## After Building

Copy these files into your `GameFolder` to run the game:

```
config.ini
diabdat.mpq
TH2data.mor
THmusic.mor   (optional)
```

Then launch `TH4.exe` directly or via the VS debugger.

---

## Troubleshooting

| Error | Fix |
|---|---|
| `Cannot find fxc.exe` | Check `FxCopDir` in `personal.props` points to a folder that contains `fxc.exe` |
| `personal.props not found` | Run `InitProject.bat` first |
| `GameFolder` errors | Make sure the path ends with a backslash `\` and the folder exists |
| `v143 toolset not found` | In VS Build Tools installer, ensure "MSVC Build Tools for x64/x86 (Latest)" is checked |
