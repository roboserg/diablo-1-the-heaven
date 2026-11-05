# Building Diablo: The Heaven on Windows 11

## Prerequisites

Install **Visual Studio Build Tools** (2022 or 2026) with the following components:

| Component | Where to find it |
|---|---|
| Desktop development with C++ workload | Workloads tab |
| MSVC Build Tools for x64/x86 (Latest) | Included automatically |
| Windows 11 SDK (any recent version, e.g. 10.0.26100.0) | Optional → Desktop development with C++ |

The project targets the **v145** toolset (Build Tools 2026). With Build Tools 2022, `scripts\build.ps1` builds with **v143** automatically. No XP support, no v141, no CMake/vcpkg/testing tools needed.

---

## Steps

### 1. Run InitProject.bat

From the repo root in a terminal:

```
scripts\InitProject.bat
```

This creates two files if they don't already exist (both ignored by git):
- `personal.props` — your local build settings, copied from `scripts\personal.props`
- `theheaven.vcxproj.user` — debugger settings

### 2. Check personal.props

The defaults usually work:

```xml
<GameFolder>$(MSBuildProjectDirectory)\build\</GameFolder>
<FxCopDir>C:\Program Files (x86)\Windows Kits\10\bin\10.0.26100.0\x86</FxCopDir>
```

- **GameFolder** — where the built `TheHeaven.exe` will be placed. Must end with a backslash `\`. The default outputs into a `build/` folder at the repo root (ignored by git).
- **FxCopDir** — directory containing `fxc.exe` (the HLSL shader compiler). Use the version folder of the Windows SDK you installed (look under `C:\Program Files (x86)\Windows Kits\10\bin\`).

### 3. Build

```
.\scripts\build.ps1           # Debug (cheats enabled)
.\scripts\build.ps1 Release   # Release
```

The script finds your Build Tools, stops a running `build\TheHeaven.exe` so it can be overwritten, copies the PDB next to the exe and removes the intermediate `Release\` folder.

To build from VS Code instead, press `Ctrl+Shift+B` and point the MSBuild task at `TheHeaven.sln` (add `/p:PlatformToolset=v143` with Build Tools 2022).

---

## After Building

Copy these files into your `GameFolder` to run the game:

```
diabdat.mpq     from Diablo 1 (required)
TH4data.mor     from The Hell 4 v0.774 (required: graphics and sounds)
config.ini      from res\config.ini (The Heaven's default settings)
```

No TH4 v0.774? Download the [backup copy](https://drive.google.com/file/d/1p50lTU8JDtfFM8lgLKiXp9vUwr3V4ROT/view) (`TH4_v0.774.7z`) and take `TH4data.mor` from it.

Then launch `TheHeaven.exe`.

---

## Autotest

`scripts\autotest.ps1` plays the real game unattended and checks recent fixes: the shipped config, magic items through the game's 5-second item check, Cain's transmute window, F9/F12 quick save and load, Griswold's and Gillian's stashes, items held or left in item windows during saves and loads, the shared stash lock between a normal Warrior and a Classic Mage, a character saved by v0.2.0 (`tests\saves\v0.2.0`) loading with every item as it was, and the errors for a missing `TH4data.mor` and for a file missing from the archives. It needs `diabdat.mpq` and `TH4data.mor` in `build\`.

```powershell
.\scripts\autotest.ps1 -Build   # build, then run every phase (about 4 minutes)
.\scripts\autotest.ps1 -Show    # watch it: the window opens on screen, still muted
```

It runs a copy of the game in `build\autotest` with its own test heroes, so your saves, shared stash and settings are never touched. The window opens off-screen, muted, never takes focus, and leaves the mouse, keyboard and gamepad alone, so you can keep using the PC.

Results go to `build\autotest`: `autotest-report.json` with every check, the `autotest-*.gif` screenshots (look at the dungeon one for pixel light), `autotest-rebuilds.csv` with every magic item the test dropped, as dropped and after each of 4 rebuilds, and an `autotest-<phase>.log` per phase showing how far it got. The exit code is 0 when every check passed. The checks themselves are in `src\AutoTest.cpp`.

---

## Troubleshooting

| Error | Fix |
|---|---|
| `Cannot find fxc.exe` | Check `FxCopDir` in `personal.props` points to a folder that contains `fxc.exe` |
| `personal.props not found` | Run `scripts\InitProject.bat` first |
| `GameFolder` errors | Make sure the path ends with a backslash `\` and the folder exists |
| `v145 toolset not found` | You have Build Tools 2022: use `scripts\build.ps1`, or pass `/p:PlatformToolset=v143` to MSBuild |
