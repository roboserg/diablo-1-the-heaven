# Diablo 1: The Hell 4 Mod Source Code

*Disclaimer: This repository is not affiliated with the The Hell team. You will find them at their [Patreon page](https://www.patreon.com/thmod).*

---

## Repository Updates

This fork enhances the original code release with:
* Updated build system for modern environments using **VS Code** (Visual Studio no longer required)
* See `BUILD.md` for detailed build instructions

---

## Original Readme from the Mod Author

**This is the Readme file to accompany The Hell 4 source.**
*Dated: March 3, 2026*

The mod is almost complete at this moment. My vision of it as an end product is mostly achieved, but there is still place for improvements.

This source remained private since *The Hell 2* times because the main programmer was against publishing it (though I was the one to get criticism for that). I believe in open source and sharing knowledge. I began TH1 mostly because *The Dark* mod developers shared their workshop for it. Now that the lead programmer is gone, I don't need to abide by this restriction anymore. Hence, opensourcing.

### Permissions & Usage
* You are free to do as you wish with this source. Create derivative works or whatever else. 
* **Naming:** Just give your derivative works a different name. Do not use "The Hell".
* **Redistribution:** You can redistribute the source, but this Readme must be included.

### Technical Details
In its core, it is *Hellfire v1.01* that was decompiled with IDA a long time ago (it began in 2007). It's not completely cleaned out—some of that *decompilatus Diablii* is still there, but it's mostly functions that are not really in need of edits.

The Hell mod developers used **Visual Studio**.

### How to Install the Source
1. Run `InitProject.bat`.
2. After this, edit `personal.props` (set the directory for the mod).
3. You may need to install **MSVC v141** compile tools in Visual Studio as well (missing this usually results in an `"FXC.exe missing"` error).

> **Note:** I don't include my workshop along with this source as it is several gigabytes in size, and I don't want to publish it.

### Modding & Contributions
Now, if you want to change something in the mod, you can do that. If you find bugs and tell me, I'd appreciate that. In particular, if you study the source and feel like you want to help me with programming, that's even better (although there is not much left to do by now).

I will create a "modding" channel in The Hell 4 Discord chat, where we can discuss everything related to making mods and programming.

* **Discord Link:** [Join The Hell Discord](https://discord.gg/AN9NukZFbN)

Have fun and good luck with whatever you're up to.

*Note: This readme may receive updates in the future if there is need for that.*

— **Mordor**