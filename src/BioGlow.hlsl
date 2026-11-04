// BioGlow.hlsl -- (new by th4) the light of a bioluminescent (GLOW) night in Tristram.
// Driver: dx_utility.cpp BioGlow_Present (window size, right after the world/UI mask).
//  1. THE RIVER in true colour: TownWater.cpp GlowRiver marks every water pixel (s3, full game
//     resolution) with its own texture brightness 1..255. That brightness picks a colour on a smooth
//     ramp from the darkest to the brightest water colour (c5 .. c6: deep blue .. pale cyan on glow
//     nights, dark .. bright crimson on blood moon nights), slow broad swells brighten and
//     darken it, and rare pale sparkles twinkle. Swells and sparkles are fixed to the map through
//     the tile (0,0) anchor (c2), so they do not slide when walking.
//  2. THE HALO: every glowing thing (river + ground lights) is counted into a quarter-resolution
//     map (s1); it is blurred here into soft blue-cyan light that bleeds onto banks and grass.
//  3. THE NIGHT TINT: world pixels are pushed toward a deeper blue.
// Everything follows the night strength (c0.x) and touches world pixels only (s2, soft weight as in
// Mist.hlsl).
// s0 = frame copy (point), s1 = glow map (L8, quarter game res, linear), s2 = world/UI mask
// (linear; 0 = UI, >= 80/255 = world), s3 = water map (L8, game res, point; 0 = not water)
// c0 = x strength 0..1, y night tint amount, zw one glow-map texel in uv
// c1 = rgb halo colour, w how much the halo may light the WATER itself (glow nights 1; blood moon 0 --
//      there the halo only lights the banks, the river keeps exactly its own crimson)
// c2 = xy screen position of tile (0,0) in game px, z zoom (1 or 2), w clock in seconds
// c3 = xy game resolution
// c4 = rgb night tint multiplier (glow nights bluer, blood moon redder; amount in c0.y)
// c5 = rgb darkest water colour, w how much of the water becomes that colour (glow 0.08, blood 0.45)
// c6 = rgb brightest water colour, w the most the water colour may grow with the night (glow
//      nights and blood moon 1 = all the way; lower stops it part of the way into the night)
// c7 = rgb sparkle colour on the water
// c8 = rgb darkest leaf colour, w how much of a leaf becomes leaf colour; c9 = rgb brightest leaf
//      colour (glow nights pink, blood moon gold-orange-red; dx_utility.cpp GLOW_LEAF_* / BLOOD_LEAF_*)
// c11 = (th4) DYNAMIC ZOOM: the maps s1 / s3 / s4 are in the DRAWN frame (as the CPU built them); their uv is
//       uv * c11.x + c11.yz (dx_utility.cpp THE VIEW; 1, 0, 0 without the zoom). s0 and s2 are the screen.
// c10 = rgb GROUND WARMTH multiplier, w amount (blood moon only, 0 otherwise); s4 = ground map (L8,
//      game res, point; 255 = a town tile pixel, palette entries 1..127 -- the half the night blend
//      darkens; heroes, NPCs, cows, items and the UI use the other half, so they are never warmed)
// (the water colours and tint come from dx_utility.cpp BioGlow_Present: GLOW_* / BLOOD_*)
sampler2D frame : register(s0);
sampler2D glow : register(s1);
sampler2D mask : register(s2);
sampler2D water : register(s3);
float4 prm : register(c0);
float4 haloCol : register(c1);
float4 anchor : register(c2);
float4 res : register(c3);
float4 tintRgb : register(c4);
float4 waterDeep : register(c5);
float4 waterBright : register(c6);
float4 sparkCol : register(c7);
float4 leafDeep : register(c8);
float4 leafBright : register(c9);
float4 warm : register(c10);
float4 vw : register(c11);
sampler2D ground : register(s4);

// Glowing leaves (TownWater.cpp GlowLeaves marks them in the same map with codes 128..255; water is
// 1..127): a ramp c8 .. c9 from the leaf's own brightness, breathing slowly.

float hash(float2 p) { return frac(sin(dot(p, float2(127.1, 311.7))) * 43758.5453); }

float4 main_bioglow(float2 uv : TEXCOORD0) : COLOR
{
	float3 c = tex2D(frame, uv).rgb;
	float worldW = saturate(tex2D(mask, uv).r * (255.0 / 80.0));
	if (worldW <= 0.0)
		return float4(c, 1.0);
	float s = prm.x;
	float2 mv = uv * vw.x + vw.yz;		// (th4) DYNAMIC ZOOM: where the drawn-frame maps are read

	// 1. river (codes 1..127) and leaves (codes 128..255)
	float code = tex2D(water, mv).r * 255.0;
	float wv = code < 127.5 ? saturate((code - 1.0) / 126.0) : 0.0;
	float leafW = code >= 127.5 ? 1.0 : 0.0;	// leaves skip the blue night tint below (it turned them violet)
	if (code >= 127.5) {
		float lv = saturate((code - 128.0) / 127.0);
		float2 world = (uv * res.xy - anchor.xy) / anchor.z;
		float breath = 0.85 + 0.15 * sin(anchor.w * 0.9 + world.x * 0.02 + world.y * 0.03);
		float3 lc = lerp(leafDeep.rgb, leafBright.rgb, saturate(lv * breath));
		c = lerp(c, lc, s * leafDeep.w);
	}
	if (code > 0.5 && code < 127.5) {
		float2 world = (uv * res.xy - anchor.xy) / anchor.z;	// 1x world px
		float t = anchor.w;
		float swell = sin(world.x * 0.045 + t * 0.55) * sin(world.y * 0.09 - t * 0.35);
		float lum = saturate(wv * (0.85 + 0.30 * swell));
		float3 wc = lerp(waterDeep.rgb, waterBright.rgb, lum * lum * (3.0 - 2.0 * lum));
		float2 cellp = floor(world * 0.5);
		float spark = step(0.9965, hash(cellp + floor(t * 5.0)));
		wc = lerp(wc, sparkCol.rgb, spark * 0.9);
		c = lerp(c, wc, min(s, waterBright.w) * waterDeep.w);
	}

	// 1b. GROUND WARMTH (blood moon): floor and stones lose the cold blue of the night -- town tile
	// pixels only (s4), never the river or leaves (already coloured above) or any sprite
	if (warm.w > 0.0 && code < 0.5) {
		float g = tex2D(ground, mv).r;
		c = lerp(c, saturate(c * warm.rgb), warm.w * s * g);
	}

	// 2. halo: two rings of 8 taps (2 and 5 glow-map texels = 8 and 20 game px) plus the centre
	float2 tx = prm.zw;
	float h = tex2D(glow, mv).r * 0.20;
	[unroll] for (int i = 0; i < 8; ++i) {
		float a = i * 0.785398;
		float2 d = float2(cos(a), sin(a) * 0.6);	// flattened like the floor
		h += tex2D(glow, mv + d * tx * 2.0).r * 0.06;
		h += tex2D(glow, mv + d * tx * 5.0).r * 0.04;
	}
	h = saturate(h * 1.6);

	// 3. night tint, then the halo on top
	float3 tinted = lerp(c, c * tintRgb.rgb, prm.y * s * (1.0 - leafW));
	float isWater = (code > 0.5 && code < 127.5) ? 1.0 : 0.0;
	float3 lit = tinted + haloCol.rgb * h * s * lerp(1.0, haloCol.w, isWater);
	return float4(lerp(tex2D(frame, uv).rgb, saturate(lit), worldW), 1.0);
}
