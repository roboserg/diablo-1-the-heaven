/*sampler tx : register(s0);
sampler pl : register(s1);

float4 main_vignetting(float2 cr : TEXCOORD0) : COLOR
{
	float4 px = tex1D(pl, tex2D(tx, cr).r);
	float ven = 1.0 - abs((cr.x - .5) * 1.6 * ((cr.y - .5) * 1.6));
	px = px * float4(ven, ven, ven, 1);
	return px;
}*/

// VignettingShader.hlsl

sampler tx : register(s0);
sampler pl : register(s1);

struct PSInput {
	float2 sp : vPos;
	float2 cr : TEXCOORD0;
};

float4 main_vignetting(PSInput i) : COLOR
{
	float w = i.cr.x / i.sp.x;
	float h = i.cr.y / i.sp.y;
	float4 px = tex1D(pl, tex2D(tx, i.cr).r);
	if (tex2D(tx, i.cr).r >= .5) return px;
	float ven = 1.0 - (abs((i.cr.x - .5) * 1.8 * ((i.cr.y - .5) * 1.8)));
	return px * float4(ven * .9, ven, ven * .95, .9);
}