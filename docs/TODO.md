# To do

Work decided on but left for later, most important first. Bugs go in [OPEN_BUGS.md](OPEN_BUGS.md), and player requests in [COMMUNITY_FEEDBACK.md](COMMUNITY_FEEDBACK.md).

## In-game bug reporter

Players rarely open GitHub issues, and bug reports on Discord are hard to follow. A **Report a bug** entry in the pause menu would send them to this repo's [GitHub issues](https://github.com/roboserg/diablo-1-the-heaven/issues), next to the ones people write by hand. About 2 days of work. Decided on 5 Nov 2026, after the v0.3.0 release.

**How it works**
1. The game takes a screenshot of the frame behind the menu. It uploads that, the current character's save, the shared stash, the log and the version to a Cloudflare Worker with a WinHTTP POST, then opens the player's browser on the Worker's page for that report.
2. On that page the player types what happened, and optionally a Discord name to reach them.
3. The Worker opens an issue labelled `in-game`. The issue has the description and version, and shows the screenshot. The page then links the player to their issue.

**Decisions**
- **Files** go in Cloudflare R2, because GitHub's API can't attach files to issues. The screenshot is public and shown in the issue. The save, stash and log stay private behind a key, since logs can hold Windows paths with the player's user name; take user names out of them anyway.
- **Author:** a GitHub App (for example `the-heaven-reports`) with issues write on this repo only, so reports show as a bot and not as roboserg. Creating it in GitHub settings is the owner's part (about 15 minutes).
- **Spam:** retro-league's limits (5 reports per player per hour, 300 per day). The `in-game` label filters these reports out of the list, and the owner can delete any issue.
- **Agents** read reports with `gh issue list --label in-game` and fetch the private files with the key.

**Start from** retro-league's feedback Worker, `F:\git\retro-league\game\service\ranked\src\feedback.mjs` and its `wrangler.toml` (D1 table, rate limit binding, admin key). Swap its Resend email for the GitHub issue, and its base64 screenshot in D1 for R2. Its browser client doesn't fit; the game side is new C++.

**Test** end to end: the autotest opens the pause menu, sends a report to a local `wrangler dev`, and checks the files and the issue body it would have created.

## Wider item pool per class

From the unmerged branch `feat/0.3.0-more-base-unique-items` (roboserg, 30 Mar 2026), never released. Its one commit, af5bc92, runs `tools/fix_class_mask.py` over `src/BaseItems.cpp`. Base items limited to a subclass become available to its whole class, so every Mage subclass gets the Necromancer's items, for example. Items for two classes become available to all. On the old TH4 base, the items every class can use went from 694 to 883 (Mage +189, Archer +150, Rogue +127, Monk +107, Savage +96, Warrior +87). The mask decides both who can use an item and what drops for them (`GetRequiredClassMask` in `src/Item.cpp`), so this is a balance change, not a fix. Left out of v0.3.0 on 5 Nov 2026.

- **Nobody asked for it** in The Heaven's channels. The nearest are AG's All Drop game changer, which Masterteo89 praises (see [IDEAS_FROM_OTHER_MODS.md](IDEAS_FROM_OTHER_MODS.md)), and Rageweaver's unanswered question about TH4's 70% smart drop and 30% parent class drop (see [COMMUNITY_FEEDBACK.md](COMMUNITY_FEEDBACK.md)).
- **Redo it rather than merge the branch:** `BaseItems.cpp` changed with the v0.774 rebase, so rerun the script on the new table. `main` already has the script that charts the class masks (67ee2c8).
- **Test** with the autotest: the shared stash lock and the morph checks must still pass, since it changes which items a class can use.
