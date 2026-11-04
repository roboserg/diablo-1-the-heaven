// Breath.hlsl -- (new by th4) COLD BREATH: soft puffs of vapour (config ColdBreath 0..10, default 5).
// Breath.cpp decides who breathes and where the puffs are (it notes the drawn sprites, finds their heads
// and keeps the puffs fixed to the world); the driver is dx_utility.cpp Breath_Present (ATMOSPHERE
// block), which passes the live puffs as constants and draws ONE SQUARE PER PUFF (each pixel in it still
// sums every puff, so overlaps match a full-window pass -- only far cheaper). WORLD pixels only (the
// world/UI mask, same soft test as Mist.hlsl). Each puff is a soft round blob; a slowly drifting value
// noise fixed to the ground breaks it into wisps; its brightness comes from the scene around it (four
// taps), so vapour in a lit place is pale and in a dark corner stays dim -- and it flashes with lightning.
//
// s0 = frame copy (point, clamp), s1 = world/UI mask (linear; m / (80/255) = 0..1 world weight)
// c0 = xy game resolution, z zoom (1 or 2), w clock in seconds (ATMOSPHERE clock, stops on pause)
// c1 = xy anchor (game px, tile 0,0), z opacity 0..1.6 (ColdBreath x 0.16), w number of puffs
// c2 .. c49 = the puffs: xy centre (screen game px), z radius (game px; NEGATIVE = toxic fumes of the
//             fungi, Breath.cpp MOUTH_TOXIC: drawn green), w opacity 0..1
sampler2D frame : register(s0);
sampler2D mask : register(s1);
float4 g : register(c0);
float4 a : register(c1);
float4 puffs[48] : register(c2);

// (th4) HASH WITHOUT SINE, wrapped to 0..288 first -- the same function as CloudShadow / Mist / Frost /
// TreeSway.hlsl (see the note in CloudShadow.hlsl); every caller passes WHOLE numbers.
float hash(float2 p)
{
	p = p - 289.0 * floor((p + 0.5) / 289.0);
	float3 p3 = frac(float3(p.xyx) * 0.1031);
	p3 += dot(p3, p3.yzx + 33.33);
	return frac((p3.x + p3.y) * p3.z);
}
float vnoise(float2 p)
{
	float2 i = floor(p), f = frac(p);
	float2 u = f * f * (3.0 - 2.0 * f);
	return lerp(lerp(hash(i), hash(i + float2(1, 0)), u.x), lerp(hash(i + float2(0, 1)), hash(i + float2(1, 1)), u.x), u.y);
}

float4 main_breath(float2 uv : TEXCOORD0) : COLOR
{
	float3 c = tex2D(frame, uv).rgb;
	float worldW = saturate(tex2D(mask, uv).r * (255.0 / 80.0));
	if (worldW <= 0.0)
		return float4(c, 1.0);
	float2 px = uv * g.xy;
	float v = 0.0, tox = 0.0;							// all vapour; the toxic part of it
	[loop] for (int i = 0; i < 48; ++i)
	{
		if (i >= a.w)
			break;
		float4 p = puffs[i];
		float2 d = (px - p.xy) / abs(p.z);
		float r2 = dot(d, d);
		if (r2 < 1.0)
		{
			float f = 1.0 - r2;
			float w = f * f * p.w;
			v += w;
			tox += w * step(p.z, 0.0);					// a negative radius = toxic
		}
	}
	if (v <= 0.0)
		return float4(c, 1.0);
	float2 world = (px - a.xy) / g.z;					// fixed to the ground
	float n = vnoise(world / 3.5 + float2(g.w * 0.6, -g.w * 0.9));
	v *= 0.55 + 0.9 * n;								// wisps
	float2 t = 6.0 / g.xy;								// 6 game px
	float3 l = tex2D(frame, uv + float2(t.x, 0)).rgb + tex2D(frame, uv - float2(t.x, 0)).rgb
	         + tex2D(frame, uv + float2(0, t.y)).rgb + tex2D(frame, uv - float2(0, t.y)).rgb;
	float lum = dot(l * 0.25, float3(0.299, 0.587, 0.114));
	// lit vapour is lighter than what is behind it; the floor of 0.45 keeps a breath readable in the
	// dark -- many monster breaths start over unlit (black) ground, and a lower floor made them vanish
	float3 vapour = float3(0.90, 0.92, 0.96) * saturate(0.45 + 1.8 * lum);
	// toxic fumes: a sickly yellow-green, a little brighter in the dark so the colour still reads
	float3 fume = float3(0.50, 0.88, 0.28) * saturate(0.55 + 1.6 * lum);
	vapour = lerp(vapour, fume, tox / v);
	float alpha = saturate(v * a.z) * worldW;
	return float4(lerp(c, vapour, alpha), 1.0);
}
