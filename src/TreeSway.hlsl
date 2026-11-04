// TreeSway.hlsl -- (new by th4) TREES SWAYING IN THE WIND (config TreeSway 0..10, default 4).
// Driver: dx_utility.cpp TreeSway_Present (ATMOSPHERE block): WHEN the wind blows and how hard
// (Wind_Strength). The tree LAYERS come from TownWater.cpp BuildSwayLayers (TREE SWAY there).
// Window size, one pass, after the BioGlow pass (so the white Ice Age / glowing crowns move whole).
//
// ONLY TREE PIXELS MOVE, smoothly (sub-pixel): for each output pixel the tree colour is taken from
// where the wind pushed it from, as a bilinear blend of the four nearest TREE pixels only (s0 at
// their game-pixel centres) -- the ground, houses, sprites and the UI never move. The DRAW ORDER
// decides who may land where: a tree pixel of tree number o may land on pixel P only if
// o >= K(P), where K(P) is the first tree drawn after which P never changed (TownWater.cpp
// BuildSwayLayers) -- so a crown slides BEHIND a hero, a house or a later tree, never over them.
// A tree pixel the crown moves off shows what lay under it before the tree was drawn: its palette
// colour (s4), lit like the tree pixel was (the ratio of the drawn colour to the tree's palette
// colour, as a brightness, clamped -- the white Ice Age crowns would otherwise light it up).
// THE MOTION (Mordor's chosen look, the first one tried): the bend grows with the height above the
// tree's ground point (the weight, R of s2); a pixel just outside a crown takes the biggest weight
// found up to 4 world px upwind (and 2 downwind), so the crown's outline can move onto it -- the
// crowns move almost only sideways, so a row of taps does what a 2D widening did; a lean that
// follows gust waves rolling across the town in WORLD space (toward +x, the way the storm rain
// leans), a slower swing per crown around it, a small quick flutter of 12 px patches; the crown
// rises a touch as it leans.
//
// s0 = frame copy (linear, clamp), s1 = world/UI mask (linear), s2 = layer 1, PRE-ZOOM res (one texel per
//      world pixel = a zoom x zoom block of game pixels), POINT:
//      R = weight, G = tree palette index, B = under-tree palette index, A = tree number + 1 (0 = no tree)
// s3 = layer 2, pre-zoom res, POINT: K / 255 (255 = no tree may land here)
// s4 = palette 256 x 1 (POINT)
// c0 = xy game resolution, z zoom (1 or 2), w wind clock in seconds (stops on pause)
// c1 = xy anchor (game px, tile 0,0), z amplitude in world px (TreeSway x 0.3 x wind), w unused
// c2 = xy layer size (pre-zoom px), zw (th4) the DYNAMIC ZOOM shift in game px (0 without it): a layer texel L
//      is shown at game px L * c0.z + c2.zw (dx_utility.cpp THE VIEW); c0.z is then the wheel's factor
sampler2D frame : register(s0);
sampler2D mask : register(s1);
sampler2D lay1 : register(s2);
sampler2D lay2 : register(s3);
sampler2D pal : register(s4);
float4 g : register(c0);
float4 a : register(c1);
float4 ls : register(c2);

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
// game pixel -> the world pixel (layer texel) shown there: the one under the pixel's centre, as the zoom
// composite picks it (ZoomComposite.hlsl). With zoom 1 or 2 and no shift it is exactly floor(px / zoom).
float2 cellOf(float2 px) { return floor((px + 0.5 - ls.zw) / g.z); }
float4 L1(float2 px) { return tex2Dlod(lay1, float4((cellOf(px) + 0.5) / ls.xy, 0, 0)); }	// px = integer game pixel
float worldAt(float2 uv) { return saturate(tex2Dlod(mask, float4(uv, 0, 0)).r * (255.0 / 80.0)); }
float lum(float3 c) { return dot(c, float3(0.299, 0.587, 0.114)); }

float4 main_treesway(float2 uv : TEXCOORD0) : COLOR
{
	float3 c = tex2D(frame, uv).rgb;
	float2 gp = uv * g.xy;			// game px, continuous
	float2 P = floor(gp);
	float K = tex2Dlod(lay2, float4((cellOf(P) + 0.5) / ls.xy, 0, 0)).r * 255.0;
	float4 m = L1(P);
	if (K > 254.5 && m.a <= 0.0) return float4(c, 1.0);	// no tree here and none may land here
	if (worldAt(uv) < 0.99) return float4(c, 1.0);		// UI over the world: never touched
	// the weight here: this pixel's, or the biggest of the crowns that can reach it (upwind = -x)
	float w = m.r;
	[unroll] for (int k = -2; k <= 4; k++) {
		if (k != 0) w = max(w, L1(P - float2(k * g.z, 0)).r);
	}
	if (w <= 0.0) return float4(c, 1.0);
	// the first version's motion, at this pixel
	float2 wp = (gp - a.xy) / g.z;	// world px from tile (0,0)
	float t = g.w;
	float gust = 0.5 + 0.5 * sin((wp.x * 0.8 + wp.y * 0.35) / 240.0 - t * 1.4);
	float swing = sin(t * 2.1 + (wp.x + wp.y * 2.0) / 140.0);
	float flutter = sin(t * 8.5 + hash(floor(wp / 12.0)) * 6.2832);
	float dx = a.z * w * (0.55 * gust + 0.30 * gust * swing + 0.12 * flutter * (0.4 + gust));
	float dy = -abs(dx) * 0.12;
	if (abs(dx) * g.z < 0.02) return float4(c, 1.0);	// at rest the picture is exactly as drawn
	// where the tree colour comes from: bilinear over the 4 nearest TREE pixels that may land here
	float2 q = gp - float2(dx, dy) * g.z - 0.5;
	float2 q0 = floor(q), f = q - q0;
	float3 acc = 0; float cover = 0;
	[unroll] for (int i = 0; i < 4; i++) {
		float2 o = float2(i == 1 || i == 3 ? 1.0 : 0.0, i >= 2 ? 1.0 : 0.0);
		float wgt = (o.x > 0 ? f.x : 1.0 - f.x) * (o.y > 0 ? f.y : 1.0 - f.y);
		float2 tp = q0 + o;
		float order = L1(tp).a * 255.0 - 1.0;			// -1 = not a tree pixel
		float2 tuv = (tp + 0.5) / g.xy;
		float ok = (order >= 0.0 && order >= K - 0.5 && worldAt(tuv) >= 0.99) ? 1.0 : 0.0;
		acc += tex2Dlod(frame, float4(tuv, 0, 0)).rgb * wgt * ok;
		cover += wgt * ok;
	}
	// what shows where no tree lands: the pixel itself, or -- on a tree pixel the crown moved off --
	// what was under the tree, lit like the tree pixel
	float3 base = c;
	if (m.a > 0.0) {
		float3 treePal = tex2Dlod(pal, float4((m.g * 255.0 + 0.5) / 256.0, 0.5, 0, 0)).rgb;
		float3 bgPal = tex2Dlod(pal, float4((m.b * 255.0 + 0.5) / 256.0, 0.5, 0, 0)).rgb;
		float light = clamp(lum(c) / max(lum(treePal), 0.02), 0.0, 1.25);
		base = bgPal * light;
	}
	float3 treeCol = cover > 0.0001 ? acc / cover : base;
	return float4(lerp(base, treeCol, saturate(cover)), 1.0);
}
