// SmaaWeights.hlsl -- Antialiasing 2 (SMAA), pass 2 of 3: blending weight calculation.
// (new by th4) Part of the ANTIALIASING option (config "Antialiasing"): see dx_utility.cpp
// AntialiasPresent for the pass order, render targets and sampler states. The option runs on the
// finished frame at GAME resolution, before any upscale (FSR / SharpScaling / plain stretch), in
// both textured-quad present paths (palette and Ultra), so it combines with every FullscreenFX.
// It replaced the old FullscreenFX 6 (FXAAShader*.hlsl, a palette shader that could not be
// combined with vignette / fog / bloom); a config with FullscreenFX 6 is read as Antialiasing FXAA
// (Config.cpp).
//
// SMAA 1x (Jimenez et al., MIT licence -- see the header of SMAA.hlsli), ULTRA preset, DirectX 9
// flavour (SMAA_HLSL_3: areaTex is A8L8 and read as .ra, searchTex is L8). The reference file is
// included UNCHANGED as SMAA.hlsli; only these three thin wrappers are ours. The VS helpers of
// the reference (offset math) run here in the pixel shader, since the present paths draw a
// pre-transformed quad with one texcoord (dx_utility.cpp Fsr_Quad) and no vertex shader.
// c0 = SMAA_RT_METRICS = (1/w, 1/h, w, h) at GAME resolution.
// If fxc ever reports too many instruction slots for ps_3_0, change SMAA_PRESET_ULTRA to
// SMAA_PRESET_HIGH in all three wrappers (fewer diagonal search steps, same algorithm).
#define SMAA_HLSL_3
#define SMAA_PRESET_ULTRA
float4 rtMetrics : register(c0);
#define SMAA_RT_METRICS rtMetrics
#include "SMAA.hlsli"

// s0 = edges from pass 1 (LINEAR: the searches rely on bilinear fetches), s1 = areaTex (linear),
// s2 = searchTex (POINT). All clamp. Output: the four blending weights.
sampler2D edgesTex : register(s0);
sampler2D areaTex : register(s1);
sampler2D searchTex : register(s2);

float4 main_smaa_weights(float2 tc : TEXCOORD0) : COLOR
{
	float2 pixcoord;
	float4 offset[3];
	SMAABlendingWeightCalculationVS(tc, pixcoord, offset);
	return SMAABlendingWeightCalculationPS(tc, pixcoord, offset, edgesTex, areaTex, searchTex, float4(0.0, 0.0, 0.0, 0.0));
}
