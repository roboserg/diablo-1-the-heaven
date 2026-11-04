sampler tx : register(s0);
sampler pl : register(s1);

float4 main_fog(float2 cr : TEXCOORD0) : COLOR
{
	float4 px = tex1D(pl, tex2D(tx, cr).r);
	float fog = clamp((1.0 - abs((cr.x - 0.5) * 2.0)) * (cr.y * 4) , 0.3, 1);
	float mid = (px.x + px.y + px.z) / 3.0;
	return float4((float3(px.x, px.y, px.z) - mid) * fog + mid, fog);
}