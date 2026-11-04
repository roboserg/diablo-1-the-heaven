// FlameGlow.hlsl -- (new by th4) FLAME HALOS: a soft warm glow around the flames of torches, braziers,
// candles and burning crosses (no config, always on where the shaders run). Flames.cpp decides which
// flames are visible and how bright (it counts the flame's pixels in the drawn frame, so a torch behind
// a wall gets no glow); the driver is dx_utility.cpp FlameGlow_Present (ATMOSPHERE block), which cuts the
// halos' area into TILES and draws each tile once with only the halos that reach it (c0.z of them, in
// their original order) -- every pixel shaded once, same sum as a full-window pass over all halos.
// WORLD pixels only (the world/UI mask, same test as Breath.hlsl). Screen blend: the glow lightens what
// is under it and never clips, so it shows in a dark corridor and barely touches a lit wall.
//
// s0 = frame copy (point, clamp), s1 = world/UI mask (linear; m / (80/255) = 0..1 world weight)
// c0 = xy game resolution, z number of halos (this tile's)
// c1 .. c32  = the halos: xy centre (screen game px), z radius (game px), w brightness 0..1
// c33 .. c64 = their colours, rgb
sampler2D frame : register(s0);
sampler2D mask : register(s1);
float4 g : register(c0);
float4 halo[32] : register(c1);
float4 tint[32] : register(c33);

float4 main_flameglow(float2 uv : TEXCOORD0) : COLOR
{
	float3 c = tex2D(frame, uv).rgb;
	float worldW = saturate(tex2D(mask, uv).r * (255.0 / 80.0));
	if (worldW <= 0.0)
		return float4(c, 1.0);
	float2 px = uv * g.xy;
	float3 glow = 0.0;
	[loop] for (int i = 0; i < 32; ++i)
	{
		if (i >= g.z)
			break;
		float4 h = halo[i];
		float2 d = (px - h.xy) / h.z;
		d.y *= 0.87;									// a little taller than wide, like the flame
		float r2 = dot(d, d);
		if (r2 < 1.0)
		{
			float f = 1.0 - r2;
			glow += tint[i].rgb * (f * f * f * h.w);	// soft: half strength at a third of the radius
		}
	}
	glow = saturate(glow * worldW);
	return float4(1.0 - (1.0 - c) * (1.0 - glow), 1.0);
}
