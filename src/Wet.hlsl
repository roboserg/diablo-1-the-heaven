// Wet.hlsl -- (new by th4) WET GROUND: the normal town while it is wet (config WetGround 0..10, default 5;
// WHEN is Weather.cpp Weather_WetQ8: a storm night and the day after it). Driver: dx_utility.cpp
// Wet_Present (WET GROUND there), the first ATMOSPHERE pass of PostFxPresent, so the cloud shadows,
// rain and everything else lie on top. The puddles themselves are found and drawn in the 8-bit frame
// by TownWater.cpp (PUDDLES: darker water and the heroes' reflections); this pass adds the true-colour
// look, on world pixels only:
//   - every wet ground pixel gets darker and a little more saturated (c1.z, c1.w);
//   - puddles: the ground under the water loses its fine texture (smoothed over a few pixels) and the
//     water mirrors the sky -- c2.rgb is only the sky's TINT (cool by day, bluer at night); its
//     brightness is the GROUND's brightness around (the smoothed water / 0.7, TownWater.cpp's
//     TerrainDark) times c3.z (more by day, less at night), with slow soft CLOUDS drifting through
//     the reflection (the cloud shadows' wind). So a puddle is clearly lighter than its ground by day
//     and stays dark at night. Two looks were tried and rejected: an absolute sky colour (daytime
//     puddles looked like patches of snow) and the darkened water's own brightness as the reference
//     (the puddles all but vanished). Lightning (c3.y) lights it up. It fades in from the rim, so the
//     rim keeps the darker 8-bit water: a wet, dark edge;
//   - while it rains (c3.x), drops make small rings in the puddles: one ring per RING_CELL ground cell
//     at a time, at a hashed spot and period (the splash idea of Rain.hlsl, on the ground instead of the
//     screen), fixed to the ground like the cloud shadows (game px minus the screen position of tile
//     (0,0), / zoom, vertical axis doubled so the rings are round on the 2:1 floor).
//
// s0 = frame copy (point, clamp)
// s1 = WET MAP (point; game resolution, L8; dx_utility.cpp Wet_BuildMask): 0 = dry (UI, sprites, void),
//      64 = wet ground, 128..255 = puddle (128 = its rim .. 255 = a third of the way in or deeper)
// c0 = xy game resolution, z zoom (1 or 2), w rain clock in seconds (Weather_ClockSec, stops on pause)
// c1 = xy anchor (game px, tile 0,0), z darkening 0..1, w extra saturation 0..1
// c2 = rgb tint of the sky the puddles mirror, w puddle opacity 0..1
// c3 = x rain strength 0..1 (rings), y lightning flash 0..1, z reflection gain (day .. night), w unused
sampler2D frame : register(s0);
sampler2D wetmap : register(s1);
float4 g : register(c0);
float4 a : register(c1);
float4 s : register(c2);
float4 k : register(c3);

// (th4) HASH WITHOUT SINE, wrapped to 0..288 first -- the same function as CloudShadow / Mist / Frost /
// TreeSway.hlsl (see the note in CloudShadow.hlsl); every caller passes WHOLE numbers.
float hash(float2 p)
{
	p = p - 289.0 * floor((p + 0.5) / 289.0);
	float3 p3 = frac(float3(p.xyx) * 0.1031);
	p3 += dot(p3, p3.yzx + 33.33);
	return frac((p3.x + p3.y) * p3.z);
}

static const float RING_CELL = 18.0;	// ground px per ring cell
static const float RING_LIFE = 0.55;	// s from the drop to the ring's end
static const float REFLECT_BIAS = 0.03;	// mirrored sky = tint x (REFLECT_BIAS + c3.z x ground brightness) x clouds
static const float WATER_DARK   = 0.7;	// TownWater.cpp WATER_BRIGHTNESS / 100: the 8-bit water is this much of its ground

float vnoise(float2 p)
{
	float2 i = floor(p), f = frac(p);
	float2 u = f * f * (3.0 - 2.0 * f);
	return lerp(lerp(hash(i), hash(i + float2(1, 0)), u.x), lerp(hash(i + float2(0, 1)), hash(i + float2(1, 1)), u.x), u.y);
}

float rings(float2 p, float t)
{
	float2 id0 = floor(p / RING_CELL);
	float r = 0.0;
	[loop] for (int j = -1; j <= 1; ++j)
	{
		[loop] for (int i = -1; i <= 1; ++i)
		{
			float2 id = id0 + float2(i, j);
			float h = hash(id), h2 = hash(id + 17.0);
			float per = 0.6 + 0.8 * h2;					// s between the drops of this cell
			float lt = frac((t + h * per) / per) * per;	// s since its last drop
			float2 ctr = (id + 0.2 + 0.6 * float2(h, h2)) * RING_CELL;
			float rad = 1.0 + 7.0 * lt / RING_LIFE;
			float ring = saturate(1.0 - abs(length(p - ctr) - rad) / 0.8) * saturate(1.0 - lt / RING_LIFE);
			r += ring;
		}
	}
	return r;
}

float4 main_wet(float2 uv : TEXCOORD0) : COLOR
{
	float3 c = tex2D(frame, uv).rgb;
	float code = tex2D(wetmap, uv).r * 255.0;
	if (code < 32.0)
		return float4(c, 1.0);					// dry: the UI, sprites, void
	float l = dot(c, float3(0.299, 0.587, 0.114));
	c = lerp(float3(l, l, l), c, 1.0 + a.w) * (1.0 - a.z);	// wet: darker, a little more saturated
	if (code > 96.0)							// a puddle
	{
		float rim = saturate((code - 128.0) / 127.0);
		float w = rim * rim * (3.0 - 2.0 * rim) * s.w;
		float2 t = 2.0 / g.xy;					// 2 game px
		float3 m = (tex2D(frame, uv + float2(t.x, 0)).rgb + tex2D(frame, uv - float2(t.x, 0)).rgb
		          + tex2D(frame, uv + float2(0, t.y)).rgb + tex2D(frame, uv - float2(0, t.y)).rgb) * 0.25;
		float3 under = lerp(c, m * (1.0 - a.z), 0.6);	// the ground under the water, smoothed
		float2 world = (uv * g.xy - a.xy) / g.z;
		float2 ground = float2(world.x, world.y * 2.0);	// fixed to the ground, round on the 2:1 floor
		float clouds = vnoise((ground - g.w * float2(9.0, 4.0)) / 70.0);	// the cloud shadows' drift
		float lumG = dot(m, float3(0.299, 0.587, 0.114)) / WATER_DARK;
		float3 sky = s.rgb * (REFLECT_BIAS + k.z * lumG) * (0.75 + 0.5 * clouds) + k.y;
		c = lerp(c, lerp(under, sky, 0.65), w);	// it mirrors the sky; lightning lights it up
		if (k.x > 0.0)
			c += rings(ground, g.w) * k.x * 0.30 * (0.4 + 0.6 * rim);
	}
	return float4(saturate(c), 1.0);
}
