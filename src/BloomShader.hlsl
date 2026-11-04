sampler tx : register(s0);
sampler pl : register(s1);

static const float LIGHT_SCALE = 1.0; // A byte range is 0-255

// Helper: sample A8L8 texture at coord, look up palette, apply brightness
float4 sampleLit(float2 coord)
{
	float4 samp = tex2D(tx, coord);
	float4 c = tex1D(pl, samp.r * (255./256) + (0.5/256));
	c.rgb *= saturate(1.0 - samp.a * LIGHT_SCALE);
	return c;
}

struct PSInput{
	float2 sp : VPOS;
	float2 cr : TEXCOORD0;
};

float4 main_bloom(PSInput input) : COLOR
{
	float w = 1.0 / (1.0 / input.cr.x * input.sp.x);
	float h = 1.0 / (1.0 / input.cr.y * input.sp.y);
	float4 px = sampleLit(input.cr);

	float4 x1 = sampleLit(input.cr + float2(-w,-h));
	float4 x3 = sampleLit(input.cr + float2(w, h));
	float4 x6 = sampleLit(input.cr + float2(w,-h));
	float4 x8 = sampleLit(input.cr + float2(-w, h));

	float4 d2 = sampleLit(input.cr + float2(0, -(h/2)));
	float4 d4 = sampleLit(input.cr + float2(-(w/2), 0));
	float4 d5 = sampleLit(input.cr + float2(w/2, 0));
	float4 d7 = sampleLit(input.cr + float2(0, h/2));

	float4 m = (x1 + d2 + x3 + d4 + d5 + x6 + d7 + x8) / 8;
	m *= m;
	return clamp(px + m,0.0,1.0);
}