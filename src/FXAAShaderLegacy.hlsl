// FXAAShader.hlsl
sampler tx : register(s0);
sampler pl : register(s1);

struct PSInput {
	float2 sp : vPos;
	float2 cr : TEXCOORD0;
};

float4 main_fxaa(PSInput i) : COLOR
{
	float w = i.cr.x / i.sp.x;
	float h = i.cr.y / i.sp.y;
	float4 px = tex1D(pl, tex2D(tx, i.cr).r);

	float4 palette_check = tex1D(pl, 0.546875);
	if (palette_check.g != 0.0 && palette_check.b != 0.0) return px;

	if (tex2D(tx, i.cr).r >= .5) {
		float4 x1 = tex1D(pl, tex2D(tx, i.cr + float2(-w, -h)).r);
		float4 x3 = tex1D(pl, tex2D(tx, i.cr + float2(w, h)).r);
		float4 x6 = tex1D(pl, tex2D(tx, i.cr + float2(w, -h)).r);
		float4 x8 = tex1D(pl, tex2D(tx, i.cr + float2(-w, h)).r);

		float4 d2 = tex1D(pl, tex2D(tx, i.cr + float2(0, -h)).r);
		float4 d4 = tex1D(pl, tex2D(tx, i.cr + float2(-w, 0)).r);
		float4 d5 = tex1D(pl, tex2D(tx, i.cr + float2(w, 0)).r);
		float4 d7 = tex1D(pl, tex2D(tx, i.cr + float2(0, h)).r);

		float4 m1 = (x1 + x3 + x6 + x8) / 16;
		float4 m2 = (d2 + d4 + d5 + d7) / 32;

		return clamp(px + m1 + m2 , .0, 1.);
	}

	if (tex2D(tx, i.cr + float2(-w, -h)).r >= .5
		|| tex2D(tx, i.cr + float2(w, h)).r >= .5
		|| tex2D(tx, i.cr + float2(w, -h)).r >= .5
		|| tex2D(tx, i.cr + float2(-w, h)).r >= .5
		|| tex2D(tx, i.cr + float2(0, -h)).r >= .5
		|| tex2D(tx, i.cr + float2(-w, 0)).r >= .5
		|| tex2D(tx, i.cr + float2(w, 0)).r >= .5
		|| tex2D(tx, i.cr + float2(0, h)).r >= .5)
		px = px * .8;
	if (tex2D(tx, i.cr + float2(-w * 2, 0)).r >= .5
		|| tex2D(tx, i.cr + float2(w * 2, 0)).r >= .5)
		px = px * 0.9;

	float ven = clamp(cos((i.cr.x - .5) * 3) * cos((i.cr.y - .5) * 3), .4, 1.);

	return px *= float4(ven, ven, ven, 0.9);
}