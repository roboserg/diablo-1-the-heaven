// FXAAShaderRimRGB.hlsl — rim-emphasis variant of FXAAShaderRGB.hlsl.
// fxc hoists texture reads above uniform branches, so the rim taps cannot be
// skipped at runtime: the variant must be a SEPARATE source file. VS's project
// system rejects duplicate <FxCompile Include> items on one file ("item already
// exists under the filter"), so this wrapper defines ENRICHED_RIM + a distinct
// entry point and includes the shared body. SwitchVirtualShader selects the
// family from the rim config.
#define ENRICHED_RIM 1
#define ENTRY main_fxaa_rgb_rim
#include "FXAAShaderRGB.hlsl"
