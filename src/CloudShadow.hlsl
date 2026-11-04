// CloudShadow.hlsl -- (new by th4) THE SKY OF THE DAY over the normal town, two modes (c1.w):
//   0 CLOUD SHADOWS drifting over the town (config CloudShadows 0..10, default 9; WHEN -- day only, cloudy
//     days only, not Ice Age, not Ravenholm -- is decided in dx_utility.cpp Clouds_Active / Clouds_Day, and
//     c1.z already holds the dusk fade);
//   1 SUNLIGHT during the sunny spell of a sunny day, while the SUN RAYS shine: the town looks sunlit -- see
//     main_clouds. Drawn by dx_utility.cpp SunRays_Present, in the SAME pass as the rays (one frame copy for
//     both): this mode lights the window AROUND the rays' box, SunRays.hlsl lights the box itself with the same
//     formula and the same registers c8..c10 -- change one, change the other.
// The two never meet (a sunny day has no clouds, Clouds_Active).
// Driver of the clouds: dx_utility.cpp Clouds_Present (ATMOSPHERE block), the first pass of
// PostFxPresent after the world/UI mask, so mist, leaves, rays etc. lie on top of the shade.
// Window size, one pass: the frame copy in s0 is darkened under soft cloud shapes, WORLD pixels only
// (panels, text and the cursor stay clear -- same soft mask test as Mist.hlsl).
// The pattern is fixed to the GROUND exactly like the mist: game px minus the screen position of
// tile (0,0), / zoom, vertical axis doubled so the blobs lie flat on the 2:1 floor. It drifts with
// a pause-aware clock, so walking moves the shadows with the ground and pausing freezes them.
//
// s0 = frame copy (point, clamp), s1 = world/UI mask (linear; m / (80/255) = 0..1 world weight)
// c0 = xy game resolution, z zoom (1 or 2), w clock in seconds
// c1 = xy anchor (game px, tile 0,0), z darkness 0..1 (CloudShadows x daylight), w THE MODE (0 clouds, 1 sunlight)
// SUNLIGHT only -- c8..c10, the same registers and numbers as SunRays.hlsl (dx_utility.cpp Sunlight_Constants, from
// the SUNLIGHT_* look):
// c8  = xyz x the colour in the light, minus the shadow's; w the luminance where the light begins (below: shadow)
// c9  = xyz x the colour in the shadow; w 1 / (luminance fully in the light - c8.w)
// c10 = xyz x the luminance in the light (the saturation); w THE STRENGTH 0..1 (the spell, Weather_SunQ8)
sampler2D frame : register(s0);
sampler2D mask : register(s1);
float4 g : register(c0);
float4 a : register(c1);
float4 sunLight : register(c8);
float4 sunKeep : register(c9);
float4 sunLum : register(c10);

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
	return lerp(lerp(hash(i), hash(i + float2(1, 0)), u.x), lerp(hash(i + float2(0, 1)), hash(i + float2(1, 1)), u.x), u.y);
}

float4 main_clouds(float2 uv : TEXCOORD0) : COLOR
{
	float3 c = tex2D(frame, uv).rgb;
	float worldW = saturate(tex2D(mask, uv).r * (255.0 / 80.0));
	if (worldW <= 0.0) return float4(c, 1.0);
	if (a.w > 0.5) {
		// SUNLIGHT: the colours the light falls on get brighter, warmer and a little more saturated; the shadows
		// painted into the art (under the trees, beside the houses, the dark side of a roof) stay dark and turn a
		// little cool -- that contrast is what reads as sun. Which is which comes from the pixel's brightness: the
		// town's baked-shadow entries are 0.03..0.08 in luminance, its lit grass and paths 0.09..0.21 (town.pal,
		// measured 2026-10-09), so the edge between the two lies there (c8.w ..). Sprites get it too.
		// = lerp(c, lerp(c x shade, (lum + (c - lum) x saturation) x warm, lit), strength), the constants worked out
		// on the CPU so that SunRays.hlsl, which has no room to spare, can do it in a handful of instructions.
		float lum = dot(c, float3(0.299, 0.587, 0.114));
		float t = saturate((lum - sunLight.w) * sunKeep.w);
		float lit = t * t * (3.0 - 2.0 * t);
		return float4(lerp(c, saturate(lit * (c * sunLight.xyz + lum * sunLum.xyz) + c * sunKeep.xyz), worldW * sunLum.w), 1.0);
	}
	float2 world = (uv * g.xy - a.xy) / g.z;
	float2 ground = float2(world.x, world.y * 2.0);
	float2 drift = g.w * float2(9.0, 4.0);	// wind, ground px per second
	float n = vnoise((ground - drift) / 320.0) * 0.60
	        + vnoise((ground - drift * 1.1) / 130.0 + 7.3) * 0.30
	        + vnoise((ground - drift * 1.2) / 55.0 + 3.1) * 0.10;
	float s = smoothstep(0.47, 0.67, n);	// cloud cover with soft edges and sunny gaps
	float k = s * a.z * worldW;
	c *= (1.0 - k) * lerp(float3(1, 1, 1), float3(0.92, 0.95, 1.0), saturate(k * 2.0));	// shade is a little cool
	return float4(c, 1.0);
}
