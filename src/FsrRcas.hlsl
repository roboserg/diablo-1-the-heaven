// FsrRcas.hlsl -- AMD FidelityFX Super Resolution 1.0, RCAS pass (robust contrast-adaptive sharpening)
//
// Port of the FSR 1.0 RCAS algorithm (AMD, MIT licence: github.com/GPUOpen-Effects/FidelityFX-FSR)
// to Direct3D 9 ps_3_0, without the optional denoise. Maths follows ffx_fsr1.h FsrRcasF.
// Pass 2 of 2 (see dx_utility.cpp FsrPresent): EASU output (window size) -> back buffer, 1:1.
//
// Constants (set by FsrPresent):
//   c0.xy = 1 / texture size     c0.z = sharpness = exp2(-stops) (config FSRSharpness / 10 stops)
sampler src : register(s0);
float4 con0 : register(c0);

float3 T(float2 uv) { return tex2Dlod(src, float4(uv, 0, 0)).rgb; }

float4 main_rcas(float2 vpos : VPOS) : COLOR
{
	float2 uv = (vpos + 0.5) * con0.xy;
	//    b
	//  d e f
	//    h
	float3 b = T(uv + float2(0, -con0.y));
	float3 d = T(uv + float2(-con0.x, 0));
	float3 e = T(uv);
	float3 f = T(uv + float2(con0.x, 0));
	float3 h = T(uv + float2(0, con0.y));
	float3 mn4 = min(min(b, d), min(f, h));
	float3 mx4 = max(max(b, d), max(f, h));
	// limit the sharpening lobe so the result stays within the neighbourhood (no clipping)
	float3 hitMin = min(mn4, e) / max(4.0 * mx4, 1.0 / 32768.0);
	float3 hitMax = (1.0 - max(mx4, e)) / min(4.0 * min(mn4, e) - 4.0, -1.0 / 32768.0);
	float3 lobeRGB = max(-hitMin, hitMax);
	float lobe = max(-(0.25 - (1.0 / 16.0)), min(max(max(lobeRGB.r, lobeRGB.g), lobeRGB.b), 0.0)) * con0.z;
	float3 pix = (lobe * b + lobe * d + lobe * h + lobe * f + e) / (4.0 * lobe + 1.0);
	return float4(pix, 1.0);
}
