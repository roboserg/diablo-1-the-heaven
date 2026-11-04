// SunRays.hlsl -- (new by th4) SUN RAYS: shafts of sunlight falling through the tree crowns of Tristram on
// SUNNY days (config SunRays 0..10). Driver: dx_utility.cpp SunRays_Present (ATMOSPHERE block): WHEN
// (SunRays_Active: Weather.cpp Weather_SunQ8 -- the day roll, by day only). The tree CROWNS come from the
// same layer TreeSway uses (TownWater.cpp BuildSwayLayers, TREE SWAY there): which world pixel is a crown
// pixel, and how high above its tree's ground point it is.
//
// HOW: the light travels in ONE fixed direction, the sun of the town art (the baked shadows of the
// houses, trees and posts fall LEFT and a little up: about 0.70 px left and 0.15..0.25 px up per pixel
// of height, measured on Mordor's screenshot of the signpost and a tree by the inn), so on screen a ray
// falls down-left, about 40 degrees from vertical (c2.zw). For each world pixel the shader walks back
// toward the sun (up-right) for up to RAY_LEN world px through the crown layer: the more crown it
// passes through, the more of the light that came through those leaves reaches this pixel -- the
// nearer the leaves, the stronger. A stripe pattern across the light direction, fixed to the world,
// cuts that light into separate thin shafts; it drifts slowly and, in the wind, flickers with the
// leaves. Not every crown lets light through: a slow pattern fixed to the world (CANOPY GAPS) picks
// the places where the leaves open up, so the town gets a few clusters of shafts, not a ray behind
// every tree. The light lands on the GROUND only (s3, TownWater.cpp SUN GROUND: floor pixels nothing
// was drawn over) -- never a roof, a wall, a fence or a sprite (the river: yes). Screen blend in warm
// sunlight: the shafts show most over the dark grass under the trees. World pixels only (world/UI
// mask). Mordor's first look (2026-10-04): rays over the rooftops, too thick, too many -- the ground
// layer, the thinner / sparser stripes and the canopy gaps answer those three.
// SUNLIGHT (2026-10-09): the whole sunlit town -- lit colours brighter and warmer, baked shadows kept -- is done
// in this same pass to save a frame copy: inside the rays' box by this shader (first thing, on every world pixel),
// around it by CloudShadow.hlsl mode 1 with the same formula and registers (c8..c10; dx_utility.cpp
// SunRays_Present draws both). Part 2 (the water) gets strength 0 (c10.w). 510 of the 512 slots.
//
// s0 = frame copy (point, clamp), s1 = world/UI mask (linear; m / (80/255) = 0..1 world weight)
// s2 = layer 1 (TownWater.cpp), PRE-ZOOM res (one texel per world pixel), POINT:
//      R = height weight (0 at the trunk's foot .. 1 high in the crown), A = tree number + 1 (0 = no tree)
// s3 = the ground layer (TownWater.cpp SUN GROUND), pre-zoom res, POINT: 1 (255) = ground the light may land
//      on, 0.5 (128) = the river's water (also lit), 0.25 (64) = a crown that lets no light through
//      (TownWater.cpp kNoSunSquares), 0 = none of these
// c0 = xy game resolution, z zoom (1 or 2), w clock in seconds (ATMOSPHERE clock, stops on pause)
// c1 = xy unused (the anchor is folded into c11), z strength (SunRays x sun of the day), w wind 0..1
// c2 = xy layer size (pre-zoom px), zw the direction the light travels (unit, world px)
// c3 = x ray length (world px), y across: BANDS stripe width / LINES slot spacing (world px), z THE PART as the
//      ground codes it lights: |ground - c3.z| < c6.w -- part 0 all ground (land 1 and water 0.5), 1 all but the
//      water (land), 2 only the water (dx_utility.cpp draws part 2 after the WaterShimmer ripple, so it stays
//      still; SunRays_Present turns the part into these two numbers), w pattern: 0 = BANDS, 1 = LINES (SUN_LOOKS)
// c4 = xy BANDS stripe threshold (smoothstep lo, hi), zw CANOPY GAPS threshold (lo, hi)
// c5 = x LINES half width of a line's core (world px), y share of the line slots that hold a ray, zw unused
//      (the DYNAMIC ZOOM offset is folded into c12)
// c6 = reciprocals worked out on the CPU (dx_utility.cpp SunRays_Present), only to keep the shader inside
//      the 512 ps_3_0 instruction slots: x 1 / (shaftHi - shaftLo), y 1 / (gapHi - gapLo), z 1 / ray length,
//      w half the range of THE PART's ground codes (c3.z)
// c7 = more of them: x 1 / zoom (unused here now), yz 1 / layer size, w 1 / across (c3.y)
// c8..c10 = SUNLIGHT, the same as CloudShadow.hlsl's (its header says what each holds)
// c11 = xy screen uv -> world px (game resolution / zoom), zw the anchor: world = uv * c11.xy + c11.zw
// c12 = xy the same scale, zw the DYNAMIC ZOOM offset: layer px = uv * c12.xy + c12.zw (dx_utility.cpp THE VIEW)
sampler2D frame : register(s0);
sampler2D mask : register(s1);
sampler2D lay1 : register(s2);
sampler2D ground : register(s3);
float4 g : register(c0);
float4 a : register(c1);
float4 ls : register(c2);
float4 r : register(c3);
float4 sh : register(c4);
float4 ln : register(c5);
float4 iv : register(c6);
float4 iz : register(c7);
float4 sunLight : register(c8);
float4 sunKeep : register(c9);
float4 sunLum : register(c10);
float4 toWorld : register(c11);
float4 toLayer : register(c12);

// (th4) HASH WITHOUT SINE, wrapped to 0..288 first -- the same function as CloudShadow / Mist / Frost /
// TreeSway.hlsl (see the note in TreeSway.hlsl); every caller passes WHOLE numbers.
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

// crown at world pixel w (layer px): the CANOPY only -- the bottom of a tree (trunk, its foot) is no source,
// nor is a crown marked in the ground layer as letting no light through (kNoSunSquares)
// PERFORMANCE: two separate branches, not one "a || b" -- HLSL evaluates both sides of || (no short
// circuit), so the old form fetched the ground layer for every tap; most taps are not on a tree at all.
float crownAt(float2 w)
{
	float2 t = (floor(w) + 0.5) * iz.yz;
	float4 m = tex2Dlod(lay1, float4(t, 0, 0));
	[branch] if (m.a <= 0.0)
		return 0.0;
	[branch] if (abs(tex2Dlod(ground, float4(t, 0, 0)).r - 0.251) < 0.1)
		return 0.0;
	return smoothstep(0.08, 0.35, m.r);
}

static const int STEPS = 24;

// smoothstep(lo, hi, x) with 1 / (hi - lo) given (c6): same curve, two instruction slots fewer each
float smoothInv(float lo, float inv, float x)
{
	float t = saturate((x - lo) * inv);
	return t * t * (3.0 - 2.0 * t);
}

float4 main_sunrays(float2 uv : TEXCOORD0) : COLOR
{
	float3 c = tex2D(frame, uv).rgb;
	float worldW = saturate(tex2D(mask, uv).r * (255.0 / 80.0));
	if (worldW <= 0.0)
		return float4(c, 1.0);
	// SUNLIGHT (header): CloudShadow.hlsl main_clouds mode 1 has the same lines -- keep the two alike
	float lum = dot(c, float3(0.299, 0.587, 0.114));
	float lt = smoothInv(sunLight.w, sunKeep.w, lum);
	c = lerp(c, saturate(lt * (c * sunLight.xyz + lum * sunLum.xyz) + c * sunKeep.xyz), worldW * sunLum.w);
	float2 w = uv * toLayer.xy + toLayer.zw;	// world px (layer texel space; + the DYNAMIC ZOOM offset)
	float gv = tex2Dlod(ground, float4((floor(w) + 0.5) * iz.yz, 0, 0)).r;
	if (abs(gv - r.z) >= iv.w)
		return float4(c, 1.0);			// not ground: a roof, a wall, a sprite... (or the other part's)
	float2 D = ls.zw;					// the way the light travels
	// PERFORMANCE: the shaft pattern and the canopy gaps (below) are worked out FIRST -- they cost a few
	// hashes, while the walk toward the sun costs 48 crown taps. Wherever either is 0 (most of the
	// ground: between the stripes and in the closed canopy) the light is 0 whatever the walk finds, so
	// the walk is skipped there. Same picture as walking first: the final product is unchanged.
	// separate shafts: stripes across the light, fixed to the world; slow drift, leaf flicker in the wind
	float2 world = uv * toWorld.xy + toWorld.zw;
	float u = dot(world, float2(-D.y, D.x)) * iz.w;
	float v = dot(world, D) / 90.0;
	float shaft;
	if (r.w < 0.5)
	{
		// BANDS (Sunny_01): soft noise stripes r.y world px wide
		float n = vnoise(float2(u, g.w * 0.04 + v));
		n += (vnoise(float2(u * 2.3, g.w * 2.5)) - 0.5) * 0.35 * a.w;
		shaft = smoothInv(sh.x, iv.x, n);
	}
	else
	{
		// LINES (Sunny_02): one slot every r.y world px across the light; a slot holds a ray with chance
		// ln.y, a hair-thin line (core ln.x px each side, soft 0.7 px edge) jittered inside its slot, with
		// its own brightness, a little variation along its length and a slow shimmer (faster in the wind)
		float k = dot(world, float2(-D.y, D.x)) * iz.w;
		float id = floor(k);
		float present = step(hash(float2(id, 7.0)), ln.y);
		float bright = 0.45 + 0.55 * hash(float2(id, 13.0));
		float jit = (hash(float2(id, 29.0)) - 0.5) * 0.5;
		float dist = abs(frac(k) - 0.5 - jit) * r.y;
		float ray = present * bright * (1.0 - smoothInv(ln.x, 1.0 / 0.7, dist));
		ray *= 0.55 + 0.45 * vnoise(float2(id * 3.1, dot(world, D) / 40.0));
		ray *= 0.80 + 0.20 * vnoise(float2(id * 1.7, g.w * (0.3 + 2.0 * a.w)));
		shaft = ray;
	}
	// CANOPY GAPS: where the leaves let light through at all, judged near the source (back toward the
	// crown), on a scale of a few trees, fixed to the world
	float2 src = (world - D * (r.x * 0.35)) / 48.0;
	float gap = smoothInv(sh.z, iv.y, vnoise(src));	// open over most of the town: the shaft stripes already thin it
	[branch] if (shaft * gap <= 0.0)
		return float4(c, 1.0);			// no shaft here, or closed canopy: no light whatever the walk finds
	float2 S = float2(-D.y, D.x) * 0.5;	// half a world px across the light: two taps a step, softer edges
	// light that came through a crown on its way here: walk back toward the sun and AVERAGE the crown along
	// the way (the strongest sample alone gave every shaft band hard straight edges): a thin fringe of
	// leaves gives thin light, a thick crown full light; the nearer the leaves the more (so each shaft
	// fades out to nothing at RAY_LEN instead of ending in a hard line)
	float acc = 0.0;
	[loop] for (int k = 1; k <= STEPS; ++k)
	{
		float d = r.x * k / STEPS;
		acc += (crownAt(w - D * d + S) + crownAt(w - D * d - S)) * 0.5 * (1.0 - d * iv.z);
	}
	float lit = saturate(acc / (STEPS * 0.22));
	if (lit <= 0.0)
		return float4(c, 1.0);
	float k2 = a.z * lit * shaft * gap * worldW;
	float3 glow = saturate(float3(1.00, 0.88, 0.62) * k2);
	return float4(1.0 - (1.0 - c) * (1.0 - glow), 1.0);
}
