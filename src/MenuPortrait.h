// MenuPortrait.h
//
// (th4) TRUE-COLOUR PORTRAITS IN THE META MENU.
//
// The problem
// -----------
// Menu portraits live in 8-bit sprite sheets -- ui_art\hero_pix.pcx, gcs.pcx,
// traits.pcx -- one tall strip each, sliced by row. Being 8-bit, every portrait
// is quantised at authoring time into the single shared MenuPalette[256], the
// same 256 entries the background, the buttons and the font atlas all compete
// for. That quantisation is where the banding comes from. Nothing at runtime can
// recover it, and no per-image palette is possible: the D3D path uploads the
// whole 8-bit surface as ONE palettised texture through ONE shader pass.
//
// The approach
// ------------
// The escape hatch AnimOrbs already uses for the life/mana globes. dx_utility.cpp
// draws the palettised fullscreen quad and then, inside the same render target,
// calls the overlay hooks. Anything drawn there is a plain alpha-blended
// D3DFVF_XYZRHW quad, no pixel shader, sampled from D3DFMT_A8R8G8B8 -- full RGBA,
// zero palette loss.
//
// This works in the menus as well as in gameplay: dx_utility.cpp picks the source
// pitch with `(!gbRunGame && !LoadingScreenCEL) ? ScreenWidth : WorkingWidth`,
// which shows the menu composites into the same 8-bit surface the quad uploads.
//
// Assets
// ------
// One true-colour strip per sheet, laid out EXACTLY like the .pcx it replaces --
// portraits stacked vertically, same order, same row count:
//
//     ui_art\hero_pix.pcx   ->   data\portraits\hero_pix.png
//     ui_art\gcs.pcx        ->   data\portraits\gcs.png
//     ui_art\traits.pcx     ->   data\portraits\traits.png
//
// The name is derived from the sheet automatically, so a new sheet needs no code
// change. .png is tried first, then .jpg, so either format works.
//
// The strip MAY be a whole-number upscale of the .pcx (2x, 4x...). Scale is taken
// from the strip's own width against the control's width, and the row height
// follows from it, so a higher-resolution strip is supersampled down and looks
// sharper than the original at no extra effort.
//
// FAILS SAFE. If a strip is missing or fails to decode, nothing is drawn and the
// original 8-bit portrait -- still painted underneath by SDlgSetBitmap -- shows
// through unchanged. Sheets can therefore be converted one at a time, and a
// partial set is never a visual regression.

#pragma once

struct IDirect3DDevice9;

// Called from the SDlgSetBitmap sites that paint a portrait. `pcxName` is the
// sheet's own path (e.g. "ui_art\\hero_pix.pcx"); the strip name is derived from it.
//
// The CONTROL HANDLE is kept rather than a rect, and re-queried every frame. Paints
// happen only when the selection changes, so a snapshot would outlive the dialog and
// leave the portrait floating over whatever came next -- and re-reading the handle
// also tracks the control if it ever moves. The overlay draws only while the window
// still exists and is visible, so no close path has to remember to clear anything.
// (th4) pcxData / pcxWidth: the 8-bit sheet SDlgSetBitmap was handed for the same
// control. The overlay only draws once THAT art is actually on screen, so it can never
// appear a frame before the dialog it belongs to. Optional: a caller that leaves them
// 0 gets the old behaviour (draw as soon as the control exists).
void MenuPortrait_Set(const char* pcxName, int portraitId, HWND portraitWin,
                      const uchar* pcxData = 0, int pcxWidth = 0);

// Called when a dialog stops showing a portrait, so the overlay stops drawing.
void MenuPortrait_Clear();

// Hold the overlay off while a modal box is on top of the portrait. The window under
// it stays visible, so no amount of asking Win32 about the CONTROL will reveal that it
// is covered -- whoever puts the box up says so instead. Counted, so nesting is safe.
void MenuPortrait_Suppress(int on);

// Draw hook. Called from dx_utility.cpp straight after RenderSurface, which is the
// path the meta menu actually takes -- RenderQuadTexture, where AnimOrbs lives, is
// only reached during gameplay (see the DrawMode/gbRunGame test there).
//
// RenderSurface has just stretched the 640x480 menu from srcRect into dstRect on the
// back buffer, so the same mapping has to be applied to the portrait rect. Both are
// passed in rather than recomputed, so the overlay cannot drift from the blit.
// (th4) menu8 / menuPitch: the 8-bit menu surface RenderSurface has just presented,
// in the same 640x480 space as srcRect. Used to check the 8-bit portrait is there.
void MenuPortrait_DrawOverlay(IDirect3DDevice9* dev, const RECT& srcRect, const RECT& dstRect,
                              const uchar* menu8 = 0, int menuPitch = 0);

// Release the D3D texture. MUST be called before IDirect3DDevice9::Reset -- the
// texture is D3DPOOL_DEFAULT, and Reset fails while it is alive. Decoded strips are
// deliberately KEPT here: they are plain RAM and re-decoding them is the slow part.
void MenuPortrait_OnDeviceLost();

// Release everything, strips included. Call once at shutdown.
void MenuPortrait_Shutdown();
