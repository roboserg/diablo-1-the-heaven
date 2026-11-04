// Mist.hlsl -- (new by th4) GROUND MIST over the normal town at night.
// Driver: dx_utility.cpp Mist_Present (called from PostFxPresent right after the world/UI mask),
// which decides when it shows (Mist_Active) and passes the night strength. Window size, one pass:
// the frame is copied to s0 and redrawn with the mist mixed in over world pixels only.
//
// The mist is two octaves of value noise, fixed to the GROUND: the pixel's game position minus
// the screen position of tile (0,0) (the c9 anchor PostFxPresent computes for the water ripple),
// divided by the zoom, with the vertical axis doubled so noise blobs lie flat on the 2:1
// isometric floor instead of standing up. It drifts slowly with its own paused-with-the-game
// clock (c0.w), so walking moves the banks with the ground and pausing freezes them.
//
// s0 = frame copy (point, clamp), s1 = world/UI mask from HeatHaze_BuildMask, sampled LINEAR:
//      codes are 0 = UI and >= 80/255 for every kind of world texel (dx_utility.cpp kOut), so
//      m / (80/255) clamped is a soft 0..1 world weight. A hard "> 0.15" test here drew the
//      mask's 2x2-block steps as a mist-free sawtooth ring around the round orb tops.
// c0 = xy game resolution, z zoom (1 or 2), w mist clock in seconds
// c1 = xy anchor (game px, screen position of tile 0,0), z strength 0..1 (night x MIST_ALPHA)
// c2 = mist colour (rgb 0..1)
sampler2D frame : register(s0);
sampler2D mask : register(s1);
float4 g : register(c0);
float4 a : register(c1);
float4 mistCol : register(c2);

// (th4) HASH WITHOUT SINE, safe on every GPU. The usual frac(sin(dot(p, (127.1, 311.7))) * 43758.5453)
// is fed world coordinates in the tens of thousands here, where one rounding step of the sine's
// argument changes the result completely; a shared noise corner is computed separately by both
// neighbouring cells, and drivers that compile that arithmetic slightly differently got two
// different values for it -- the noise then stepped at every cell border (a player's cloud shadows
// broke into rectangles; Mordor's GPU never showed it). Here the lattice point is wrapped to
// 0..288 first (exact for WHOLE numbers -- every caller passes floor()ed points), so all numbers stay
// small, and the rest is Dave Hoskins' "hash without sine" (MIT). Same look, different layout.
// The same function is in CloudShadow / Mist / Frost / TreeSway.hlsl -- keep them alike.
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
	float n00 = hash(i), n10 = hash(i + float2(1, 0)), n01 = hash(i + float2(0, 1)), n11 = hash(i + float2(1, 1));
	return lerp(lerp(n00, n10, u.x), lerp(n01, n11, u.x), u.y);
}

float4 main_mist(float2 uv : TEXCOORD0) : COLOR
{
	float3 c = tex2D(frame, uv).rgb;
	float worldW = saturate(tex2D(mask, uv).r * (255.0 / 80.0));	// 0 = UI, text, cursor; 1 = world
	if (worldW <= 0.0)
		return float4(c, 1.0);
	float2 world = (uv * g.xy - a.xy) / g.z;	// 1x px from tile (0,0)
	float2 ground = float2(world.x, world.y * 2.0);
	float t = g.w;
	float n = vnoise(ground / 150.0 + t * float2(0.020, 0.006)) * 0.65
	        + vnoise(ground / 60.0 - t * float2(0.012, 0.009)) * 0.35;
	float d = smoothstep(0.40, 0.78, n);		// banks of mist with clear gaps between them
	return float4(lerp(c, mistCol.rgb, d * a.z * worldW), 1.0);
}
