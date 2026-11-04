// AnimOrbs.h
// Animated full-color (RGBA) overlays for the life & mana globes.
// Active in normal gameplay whenever the bottom panel is visible.
//
// (th4) Outside Classic, UserPanelB does NOT gate these -- it picks the mana COLOUR,
// matching the panel art chosen in Panel.cpp (menu: "Mana: blue" / "Mana: dark"):
//   UserPanelB              -> blue mana globe -> mana_orb1
//   otherwise (the DEFAULT) -> dark mana globe -> mana_orb2_black
// CLASSIC NEVER USES THESE, whatever "panel" is set to in config.ini: it keeps its
// vanilla-Diablo globes ("panel8") so it looks like the original game.
// Only the selected mana set is resident; each is ~45 MB, and it is swapped if the
// menu flips the colour at runtime.
// The life globe is red in every variant except the Ice Age panel, which draws a
// BLUE life globe -- there is no blue life art, so the life orb is skipped there
// and the static globe shows. The mana orb still animates.
//
// Frames are loaded from the mod MPQ (th4data.mor) and drawn as
// alpha-blended screen-space quads in the D3D textured-quad render path, so no
// palette quantization / quality loss occurs.
//
// Art layout in the MPQ:
//   data\orbs_v1\health_orb1\0001.png .. 1500.png   (88x88 RGBA)
//   data\orbs_v1\mana_orb1\0001.png   .. 1500.png   (88x88 RGBA)
//   data\orbs_v1\mana_orb2_black\0001.png .. 1500.png   (88x88 RGBA)

#pragma once

struct IDirect3DDevice9;

// Frame count and source dimensions are fixed by the art set.
#define ANIMORBS_FRAME_COUNT  1500
#define ANIMORBS_SIZE         88

// Globe well placement in GUI space, shared with Draw.cpp (which clears the
// alpha overlay under the wells so the semi-transparent glass never reveals an
// occluded sprite). Keep in sync with the orb quad placement in AnimOrbs.cpp.
extern const int kLifeGuiX;   // life globe well, panel X
// mana globe well, panel X
extern const int kManaGuiX;
// panel Y of orb well top
extern const int kOrbGuiY;

// Default animation rate. Tunable at runtime via AnimOrbs_SetFps() (trial & error).
#define ANIMORBS_DEFAULT_FPS  1

// Decode every PNG from the MPQ into RAM once. Safe to call repeatedly; the
// second call is a no-op. Returns true if at least the health set loaded.
bool AnimOrbs_Init();

// Release RAM buffers and any device textures.
void AnimOrbs_Shutdown();

// (Re)create the per-orb dynamic D3D textures. Call after the device is created
// and after any device reset. `dev` is the active D3D9 device.
void AnimOrbs_OnDeviceCreated(IDirect3DDevice9* dev);

// Drop device textures before a device reset.
void AnimOrbs_OnDeviceLost();

// Draw the current animation frame over both globes. Must be called from inside
// the in-game D3D quad pass, with the same render target the game quad used, so
// the orbs inherit identical scaling. No-op outside gameplay / with no panel.
// `targetW`/`targetH` are the dimensions of the surface currently being drawn to
// (ScreenWidth/ScreenHeight for the offscreen `vid`, or backbuffer desc size).
void AnimOrbs_DrawD3D(IDirect3DDevice9* dev, int targetW, int targetH);

// Runtime knob for tuning playback speed.
void AnimOrbs_SetFps(int fps);
int  AnimOrbs_GetFps();

// (th4) Stage 2: clear the alpha overlay under the animated orb glass, per
// pixel, using the current frame's alpha (same clock formula as DrawD3D).
// Called from ClearAlphaOverlayUnderUI before the composite; no-op when the
// overlay feature or the orbs are inactive.
void AnimOrbs_CaptureKeyBand();    // (th4) call before DrawRedScreen remaps the surface
void AnimOrbs_InvalidateKeyBand(); // (th4) call from anything that repaints the orb band
void AnimOrbs_ClearOverlayWells();

// (th4) Stage 3 (2026-09-05): same per-pixel clear for the colored-light tint
// (TINT_WHITE under glass pixels with alpha >= kClearGlassAlpha). Called from
// ClearTintUnderUI; no-op when idle or the orbs are inactive.
void AnimOrbs_ClearTintWells();

// True when the animated orbs are active (drawing over the globes). The engine
// can use this to skip its own globe-fill draw and avoid a doubled fill level.
bool AnimOrbs_Active();