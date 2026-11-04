# Diablo 1: The Hell 4 Mod Source Code

*Disclaimer: This repository is not affiliated with the The Hell team. You will find them at their [Patreon page](https://www.patreon.com/thmod).*

**Current source version: v0.774** (released by the TH team on October 13, 2026). The author's original readme ships alongside the source as [`__README.txt`](__README.txt), as the release terms require.

---

## Repository Updates

This fork enhances the original code release with:
* Updated build system for modern environments using **VS Code** (Visual Studio no longer required)
* See `BUILD.md` for detailed build instructions

### Versions

| Version | Released | Untouched source |
|---|---|---|
| v0.774 | October 13, 2026 | tag `v0.774` |
| v0.208 | March 3, 2026 | tag `v0.208` |

The `upstream` branch holds each source release exactly as the TH team distributed it, one commit per release. `main` is `upstream` plus this fork's build and documentation changes, so `git diff v0.208 v0.774` shows precisely what changed between releases.

---

## Original Readme from the Mod Author

**This is the Readme file to accompany The Hell 4 source v0.774.**
*Dated: October 13, 2026*

### Permissions & Usage
* You are free to do as you wish with this source. Create derivative works or whatever else.
* **Naming:** Just give your derivative works a different name. Do not use "The Hell".
* **Redistribution:** You can redistribute the source, but this Readme must be included.

### What's new compared to the March 2026 source
* Should be more convenient to use for making submods.
* All of the old Russian comments in the source were translated to English.

### Technical Details
In its core, it is *Hellfire v1.01* that was decompiled with IDA a long time ago (it began in 2007). It's not completely cleaned out—some of that *decompilatus Diablii* is still there, but it's mostly functions that are not really in need of edits.

### How to Install the Source
1. Run `InitProject.bat`.
2. After this, edit `personal.props` (set the directory for the mod).

> **Note:** I don't include my workshop along with this source as it is several gigabytes in size, and I don't want to publish it just yet.

### Modding
There is a "modding" channel in The Hell 4 Discord chat, where we can discuss everything related to making mods and programming.

* **Discord Link:** [Join The Hell Discord](https://discord.gg/AN9NukZFbN)

Have fun and good luck with whatever you're up to.

*Note: This readme may receive updates in the future if there is need for that.*

— **Mordor**
