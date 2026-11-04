// ItemOverlay.h
// (th4) True-colour replacement art for inventory item icons.
//
// WHY
// Item icons live in 8-bit CEL sheets quantised into the game's static palette, so
// roughly 128 usable colours cover every item in the mod. Light blue, clean green
// and purple are effectively impossible. Rather than repalette anything, this draws
// an RGBA image OVER the icon the engine already blitted -- the same trick
// MenuPortrait uses for menu portraits and AnimOrbs uses for the life/mana globes.
//
// NAMING
// One folder per CEL sheet, and the file is named with the SAME number written in
// BaseItems.cpp -- the part before the "+168" or "+3885", not the sum:
//
//     data\items\th2\<N>.png        BaseItems "N+168"   (TH2objcurs.cel)
//     data\items\th3\<N>.png        BaseItems "N+3885"  (TH3objcurs.cel)
//     data\items\inv1\<N>.png       BaseItems "N"       (Invobj1 / Classic)
//
// Example: Mana Stone Shard is "1858+168" in the table, so it is th2\1858.png.
//
// Internally the queue still carries the composite runtime id (table value plus
// CM_12_ITEMS_PICS_START) because that is unique across every sheet and makes a
// safe cache key. Load() splits it back into (sheet, number) using exactly the
// same thresholds GetItemSprites uses. Capture the id BEFORE calling
// GetItemSprites -- it takes an int& and subtracts the sheet base in place.
//
// OPT-IN
// A missing file is not an error. File_Open(..., ONE_TRY) reports absence quietly,
// nothing is queued, and the original CEL the engine already drew stays on screen.
// So sprites can be converted a few at a time, forever, with no flag day.
//
// SCOPE
// Inventory, equipment slots, cursor, and the stash/trade/crafting panels -- every
// caller of GetItemSprites except the ground. Items lying in the dungeon are lit
// through the palette and would glow at full brightness if overlaid, so they stay
// on CEL.
//
// LIFETIME
// The queue is per-frame: callers add rects while the 8-bit surface is being built,
// ItemOverlay_DrawD3D() flushes and clears it after that surface is uploaded. There
// is no state that survives a frame, so nothing can leak the way the portrait
// overlay could -- no Suppress() equivalent is needed here.

#pragma once

struct IDirect3DDevice9;

// Record one icon to be overlaid this frame. Call immediately after the CEL_Draw
// that painted it, with the SAME x/y/width and the pre-GetItemSprites id.
// y is the CEL bottom edge, matching CEL_Draw's own convention.
// outlineColor is the palette index CEL_Draw was told to outline the icon with when the
// cursor is over it (-1 when there is none). CEL_Draw paints that outline in the same
// pass as the icon, so our quad would bury it; passing it here lets those exact pixels be
// lifted back out and drawn on top.
// Returns true if the icon was recorded; false when the queue is full or the size is
// degenerate. Only the cursor site cares -- see ItemOverlay_MarkQueuedOnCursor.
// (th4) drawFlag: pass the SAME flag word the paired ItemCEL_Draw call gets. Only R_RED
// is read -- an item whose requirement is not met is tinted by the replacement art the
// same way CEL_DrawTo tints the 8-bit sprite. Defaults to 0, so a site that has not been
// converted keeps exactly its present behaviour (no tint) rather than getting a wrong one.
bool ItemOverlay_Queue(int graphicValue, int x, int yBottom, int w, int h, bool mirrored,
                       int outlineColor = -1, int drawFlag = 0);

// Drop-in for CEL_Draw at the item-icon sites. If true-colour art exists for this sprite
// the body is NOT drawn -- only the outline -- because otherwise the 8-bit sprite shows
// around the edges wherever the replacement's silhouette is smaller. With no art it
// forwards to CEL_Draw unchanged, so unconverted items behave exactly as before.
// Pass the id BEFORE GetItemSprites mutated it, same value as ItemOverlay_Queue.
void ItemCEL_Draw(int overlayGfx, int x, int y, char* celBuf, unsigned frameNum, int drawWidth,
                  int offsetStart, int offsetEnd, int flag, int outlineColor);

// (th4) True when replacement PNG art exists for this graphic — in that case
// ItemCEL_Draw draws no 8-bit body (the PNG is drawn post-quad instead).
bool ItemOverlay_HasReplacementArt(int graphicValue);

// Register a rectangle that DrawBoxTransparent just darkened (tooltip backgrounds and
// anything else that uses it). Icons inside it are drawn dimmed to match, by dimFactor
// -- each darkening primitive dims by a different amount, and none of them is a clean
// multiplier on true colour, so the numbers at the call sites are matched by eye:
// 0.35 for DrawBoxTransparent (remaps into the bottom quarter of a 16-step ramp),
// 0.50 for the dither boxes (DrawDialogBox, DrawOnScreenMessage: literal black on
// every other pixel). Screen space,
// i.e. the coordinates BEFORE Screen_LeftBorder/TopBorder are added. Per-frame, like
// the icon queue.
void ItemOverlay_QueueDimRect(int x, int y, int w, int h, float dimFactor);

// Call at the END of DrawBoxTransparent, once the rectangle has been darkened but
// before any text is written into it. Takes a copy of those pixels so that whatever is
// drawn on top afterwards (the tooltip text, its border) can be lifted back out and
// re-drawn above the icons.
void ItemOverlay_SnapshotDimRect();

// (th4) Mark the entry just queued as the item being dragged on the cursor. Call ONLY when
// the matching ItemOverlay_Queue returned true -- it marks the last entry in the queue, so
// after a Queue that bailed it would flag some unrelated inventory item instead.
//
// Everything else -- items lying in the inventory, worn on the body, in the belt -- is drawn
// BEFORE the claimed rectangles are patched back, so the in-game menu ends up on top of them.
// The dragged item is drawn after that pass instead, because it follows the cursor and has to
// stay above everything.
void ItemOverlay_MarkQueuedOnCursor();

// (th4) Drop the queue retained for replay across a palette fade. Call it wherever the
// screen's contents are torn down (ClearScreen), so a fade that follows cannot bring
// the previous screen's icons back for a frame.
void ItemOverlay_ForgetHeldQueue();

// (th4) Called by MainDrawFunc at the point the panels are (re)drawn into
// WorkingSurface. It is the one honest signal that THIS frame's queue describes what
// is on screen -- a present without it (a fade step, DrawNotGameplayScreen) is a
// re-conversion of the last composed frame and must replay that frame's queue.
void ItemOverlay_FrameComposed();

// (th4) True if replacement art exists for this id (static or animated).
bool ItemOverlay_HasArt(int graphicValue);

// (th4) Queue the art as a ghost: 50% screen-space checkerboard, same as R_TRANS in the 8-bit
// path, no hover outline. Used for the off-hand slot under a two-handed weapon, where the
// engine draws CEL_DrawShadow otherwise.
bool ItemOverlay_QueueGhost(int graphicValue, int x, int yBottom, int w, int h, bool mirrored);

// Called by the item-on-cursor site with that item's sprite id. If the dragged item has
// replacement art, the cursor pass stands down -- re-lifting it out of the 8-bit surface
// would paint the old art straight back over the overlay. If it has NO art it is 8-bit
// only, and the cursor pass must still run, or dragging it across a converted item leaves
// it buried under that item's overlay.
void ItemOverlay_MarkCursorItem(int graphicValue);

// Draw everything queued this frame, then clear the queue. Called from
// RenderQuadTexture, next to AnimOrbs_DrawD3D, so it inherits identical scaling.
void ItemOverlay_DrawD3D(IDirect3DDevice9* dev);

// (th4) Mark the opaque pixels of the icons drawn this frame as UI (0) in a world/UI mask built by comparing the
// 8-bit frame with the world snapshot -- item art is not in the 8-bit frame. half = one mask texel per 2 x 2 game
// px. Callers: dx_utility.cpp HeatHaze_BuildMask, Grass_BuildMask, Wet_BuildMask. Details at the definition.
void ItemOverlay_MarkUi(unsigned char* mask, int pitch, int w, int h, bool half);

// (th4) Tell the overlay DrawRedScreen (Panel.cpp) has just remapped the whole working
// surface through `xlat` (a LightTable row; indices below minIndex left alone -- 0x20 in
// Hell, 0 elsewhere). Icons queued so far are reddened the same way and their cover
// snapshots are remapped to match; without it every queued icon is hidden by its own
// cover patch on pause and death. Full explanation beside the definition in ItemOverlay.cpp.
void ItemOverlay_OnRedScreen(const unsigned char* xlat, int minIndex);

// Drop GPU textures on device loss. Decoded pixels are kept, so nothing re-reads
// from disk afterwards.
void ItemOverlay_OnDeviceLost();

// Release textures and decoded pixels. Process shutdown only.
void ItemOverlay_Shutdown();
