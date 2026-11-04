// AaFxaa.hlsl -- Antialiasing 1 (FXAA): luminance edge smoothing on the finished RGB frame.
// (new by th4) Part of the ANTIALIASING option (config "Antialiasing"): see dx_utility.cpp
// AntialiasPresent for the pass order, render targets and sampler states. The option runs on the
// finished frame at GAME resolution, before any upscale (FSR / SharpScaling / plain stretch), in
// both textured-quad present paths (palette and Ultra), so it combines with every FullscreenFX.
// It replaced the old FullscreenFX 6 (FXAAShader*.hlsl, a palette shader that could not be
// combined with vignette / fog / bloom); a config with FullscreenFX 6 is read as Antialiasing FXAA
// (Config.cpp).
//
// Same filter as the retired FXAAShader.hlsl (5-tap cross, blend ACROSS the stronger second
// difference, skip low-contrast areas), but it reads the finished frame instead of doing the
// palette lookup itself, so it no longer replaces the scene shader.
// s0 = the frame (point, clamp); c0.xy = 1 / game resolution.
sampler2D frameTex : register(s0);
float4 texel : register(c0);

float luma(float3 c)
{
	return dot(c, float3(0.2126, 0.7152, 0.0722));	// ITU-R BT.709
}

float4 main_aafxaa(float2 tc : TEXCOORD0) : COLOR
{
	float4 px = tex2D(frameTex, tc);
	float4 nU = tex2D(frameTex, tc + float2(0, -texel.y));
	float4 nD = tex2D(frameTex, tc + float2(0,  texel.y));
	float4 nL = tex2D(frameTex, tc + float2(-texel.x, 0));
	float4 nR = tex2D(frameTex, tc + float2( texel.x, 0));

	float lC = luma(px.rgb), lU = luma(nU.rgb), lD = luma(nD.rgb), lL = luma(nL.rgb), lR = luma(nR.rgb);
	float lMin = min(min(min(lC, lU), min(lD, lL)), lR);
	float lMax = max(max(max(lC, lU), max(lD, lL)), lR);
	if (lMax - lMin < 0.0625)	// flat area: leave texture detail alone
		return float4(px.rgb, 1.0);

	float hGrad = abs(lL + lR - 2.0 * lC);	// large: the edge runs vertically
	float vGrad = abs(lU + lD - 2.0 * lC);	// large: the edge runs horizontally
	float factor = saturate(2.0 * max(hGrad, vGrad)) * 0.45;
	float3 across = (hGrad > vGrad) ? (nL.rgb + px.rgb + nR.rgb) : (nU.rgb + px.rgb + nD.rgb);
	return float4(lerp(px.rgb, across / 3.0, factor), 1.0);
}
