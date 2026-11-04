// RetroCell.hlsl -- (new by th4) RETRO MODES, cell families (ZX Spectrum, ASCII, Teletext).
// Thin wrapper: the body is Retro.hlsl -- edit that, not this. It exists because the colour
// families and the cell families in one shader pass the 512-slot ps_3_0 minimum, and Visual
// Studio rejects two <FxCompile> items on one file (same reason as the *ShaderRimRGB.hlsl wrappers).
#define RETRO_CELL
#include "Retro.hlsl"
