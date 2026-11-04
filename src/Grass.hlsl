// Grass.hlsl -- (new by th4) BLADES OF GRASS (config GrassBlades 0..10, default 10).
// Driver: dx_utility.cpp Grass_Present (ATMOSPHERE block): the look's numbers, and when the wind blows
// (Wind_Weather -- the same wind that moves the trees, whatever TreeSway says). Where the blades stand and
// where they may show: TownWater.cpp GRASS (the layer, the block map, the grass share of each map cell).
// Window size, one quad over the grass's box, the FIRST pass of PostFxPresent (so the wet ground, the clouds,
// the mist, the rain, the glow-night halo and the sun rays all lie over the blades).
//
// WHAT IT DRAWS: every world pixel of grass (layer codes 2..4) may hold the ROOT of one blade. Whether it
// does, and the blade's height, lean, width, brightness and flutter phase, come from a hash of the root's map
// position (measured from the centre of map cell (0,0), c1.xy), so every blade belongs to its spot of ground
// and is the same blade in every frame. A blade is a thin curved stroke rising straight up from its root
// (grass is vertical in the world, so vertical on screen), tapering to a point, anti-aliased at WINDOW
// resolution, so it stays smooth under the upscale and the wheel zoom. Its colour is the screen colour of its
// root pixel -- lit, darkened by the night, in whatever palette the town has (hay-coloured grass grows
// hay-coloured blades) -- darker at the foot, lighter at the tip. Read from the frame at GAME resolution (s7),
// never from the stretched window frame: there every pixel carries up to a quarter of its neighbour, and roots
// beside white letters or lit windows grew whitish blades (dx_utility.cpp THE GAME-RESOLUTION FRAME).
// HOW A PIXEL FINDS ITS BLADES (a gather, no geometry): blades only grow UP, so the roots that can reach a
// pixel lie in its own row and in the rows below it, as far down as the tallest blade. In each such row the
// wind's bend at that height is known, so only the 5 roots around where a blade would have to stand are
// tried. Rows are walked from the pixel's own row (farthest back) toward the camera, and every blade found
// is laid OVER the colour so far: the nearer blade ends up in front.
// WHAT A BLADE PER PIXEL MUST NOT DEPEND ON: presence, height and lean come from the root alone (its hash and
// its cell share, s3 read AT THE ROOT); were they read at the pixel, a blade near a threshold would show on
// some of its pixels and not on others. The wind is read at (pixel x, root row) -- the same for every pixel of
// one blade but for its own small bend.
// WHERE: only on visible ground (layer code >= 1) -- never on a sprite, a wall, an item, the water or the UI.
// THE UI, EXACTLY (s1 = dx_utility.cpp Grass_BuildMask's WORLD MAP, one texel per game pixel, read POINT): the
// shared half-resolution world/UI mask, read smoothed, left a two-pixel fringe without blades round the mouse
// cursor (drawn into the 8-bit frame like all the UI) that went wherever the cursor went. And a blade whose
// ROOT the UI covers must not change either: its colour then comes from the root's palette index (s5) through
// the palette shown now (s6), lit like the pixel being drawn (the brightness of its screen colour against its
// own palette colour) -- in plain palette mode the very same colour. The cursor passes over the grass and no
// blade changes: it is only drawn on top of them.
// THE WIND: a blade's tip is pushed bend * (h / HMAX)^2 sideways at height h (stiff at the foot, bending at
// the tip); bend comes from the gust waves that also move the trees (TreeSway.hlsl: rolling toward +x
// across the town), a slower swing, a quick ripple of short waves running through the grass, and a flutter
// per blade. A bent blade reaches a little less high, and its tip catches a little light in a gust. No wind
// (c1.z = 0): the blades stand still -- they are there, they just do not move.
// SHADOWS ON THE BLADES: the art's baked shadows (trees, houses, the signpost) lie flat on the ground, and the
// blades growing over them would hide them (tried: thin branch shadows vanished under the lit blades in front).
// So a point of a blade at height h takes the shadow of the ground spot its sunlight comes from -- the point
// moved by h * c5.xy, the town art's own light direction (dx_utility.cpp SUN_DIR: 0.70 px left and about 0.20
// up per px of height). Shadow there (layer code 4) and a lit root: the point is darkened (c5.z); a root in
// shadow and light there: the point takes the lit ground's colour. The shadows go on up the blades.
//
// s0 = frame copy (POINT), s1 = the WORLD MAP, game res, POINT: 1 = world, 0 = UI (THE UI, EXACTLY),
// s2 = THE LAYER, pre-zoom res (texel = world px), POINT:
//      code / 255 -- 0 nothing may show here, 1 ground, 2 green grass, 3 a speck of the grass texture,
//      4 grass in a baked shadow
// s3 = grass share of the 112 x 112 map cells (u = row, v = col), LINEAR: 0..1
// s4 = BLOCK map (c3.w = 1 / its block size in layer px), POINT: 1 = a blade may reach into this block
// c0 = xy game resolution, z layer px -> game px (Alt+Z's 2 x the wheel zoom), w wind clock in s (stops on pause)
// c1 = xy centre of map cell (0,0) in layer px, z bend in world px at the tip of the tallest blade in a full
//      gust (0 = calm), w window px per layer px (the anti-aliasing)
// c2 = xy layer size (pre-zoom px), zw (th4) the DYNAMIC ZOOM shift in game px: a layer texel L is shown at
//      game px L * c0.z + c2.zw (dx_utility.cpp THE VIEW)
// c3 = x HMAX, the tallest blade (world px), y 1 / HMAX, z rows to walk (HMAX + 2), w 1 / block size
// c4 = xy block map size, z wind 0..1 (the tips' sheen), w how many of the possible blades grow (0..1)
// c5 = xy the sun's shift on the ground per px of height (SHADOWS ON THE BLADES), z how dark a shadowed blade
//      point is, w THE MODE (config GrassResolution, dx_utility.cpp Grass_Present):
//        0 the blades drawn over the frame (s0) at WINDOW resolution -- one pass, the sharpest, the heaviest
//        1 the GRASS LAYER: the same blades drawn into a small target (the game's resolution, or half of it); out =
//          the blades' colour PREMULTIPLIED by their coverage, a = the coverage. a.w is then target px per layer px
//        2 COMPOSE (its own entry point, main_grasscompose: GrassCompose.hlsl = #define GRASS_COMPOSE + #include this
//          file -- the slot limit; EDIT THIS FILE): the window frame (s0) with that layer (s8, LINEAR) laid over it --
//          only on visible ground outside the UI, the same tests mode 0 makes per pixel, so a sprite, a panel or the
//          cursor never gets grass smeared onto it by the stretch.
//      The layer is exact: lerping blade after blade over the frame (mode 0) equals frame * (1 - a) + the layer, so
//      modes 1 + 2 at the window's own size would give mode 0's picture. The measured cost (GPU timestamps,
//      2026-10-09, RX 6900 XT, 1080p): mode 0 about 4 ms of GPU a frame, 15 x every other town effect together --
//      on built-in graphics that was the town's frame rate.
// s5 = the floor's palette index of each layer texel (right on all visible ground), POINT
// s6 = the palette shown now, 256 x 1, POINT
// s7 = the frame at game resolution (or the window frame, when it was not stretched), POINT
sampler2D frame : register(s0);
sampler2D world : register(s1);
sampler2D lay : register(s2);
sampler2D cells : register(s3);
sampler2D blk : register(s4);
sampler2D idx : register(s5);
sampler2D pal : register(s6);
sampler2D game : register(s7);
sampler2D low : register(s8);	// THE MODE 2: the grass layer, LINEAR
float4 g : register(c0);
float4 a : register(c1);
float4 ls : register(c2);
float4 k : register(c3);
float4 bs : register(c4);
float4 sun : register(c5);

// (th4) HASH WITHOUT SINE (Dave Hoskins, MIT), 2 -> 4. The point is wrapped to 0..1008 first (exact for the
// WHOLE numbers passed here), so the products stay small and every GPU gets the same blades -- the same
// precaution as the hash in TreeSway / CloudShadow / Mist.hlsl. The pattern repeats every 1009 world px.
float4 hash42(float2 p)
{
	p = p - 1009.0 * floor((p + 0.5) / 1009.0);
	float4 p4 = frac(p.xyxy * float4(0.1031, 0.1030, 0.0973, 0.1099));
	p4 += dot(p4, p4.wzxy + 33.33);
	return frac((p4.xxyz + p4.yzzw) * p4.zywx);
}
// is game pixel gp (continuous game px) world, not UI? (THE UI, EXACTLY)
bool isWorld(float2 gp) { return tex2Dlod(world, float4((floor(gp) + 0.5) / g.xy, 0, 0)).r > 0.5; }
float lum(float3 c) { return dot(c, float3(0.299, 0.587, 0.114)); }
// the palette colour (s6) of layer texel t's floor index (s5)
float3 palAt(float2 t) { return tex2Dlod(pal, float4((tex2Dlod(idx, float4((t + 0.5) / ls.xy, 0, 0)).r * 255.0 + 0.5) / 256.0, 0.5, 0, 0)).rgb; }
// the colour of game pixel gp (continuous game px), exactly that one pixel (s7)
float3 gameAt(float2 gp) { return tex2Dlod(game, float4((floor(gp) + 0.5) / g.xy, 0, 0)).rgb; }
// the screen colour of layer texel t: the frame where the world shows there, else its palette colour times lit
float3 groundColour(float2 t, float lit)
{
	float2 gp = (t + 0.5) * g.z + ls.zw;	// game px of the texel's centre on screen
	if (isWorld(gp)) return gameAt(gp);
	return palAt(t) * lit;
}
// the layer code at a whole texel t; 0 off the layer (CLAMP would repeat the edge texels as phantom roots)
float codeAt(float2 t)
{
	if (t.x < 0.0 || t.y < 0.0 || t.x >= ls.x || t.y >= ls.y) return 0.0;
	return floor(tex2Dlod(lay, float4((t + 0.5) / ls.xy, 0, 0)).r * 255.0 + 0.5);
}
// the grass share of the map at a point m (world px from the centre of cell (0,0)): map cell (r, c) has its
// centre at m = (32 (r - c), 16 (r + c)), and texel (r, c) of s3 holds that cell's share
float shareAt(float2 m) { return tex2Dlod(cells, float4(float2(m.x / 32.0 + m.y / 16.0 + 1.0, m.y / 16.0 + 1.0 - m.x / 32.0) * (0.5 / 112.0), 0, 0)).r; }

#ifdef GRASS_COMPOSE
float4 main_grasscompose(float2 uv : TEXCOORD0) : COLOR	// THE MODE 2: the grass layer over the window frame
{
	float3 c = tex2D(frame, uv).rgb;
	float2 P = floor((uv * g.xy - ls.zw) / g.z);
	if (P.x < 0.0 || P.y < 0.0 || P.x >= ls.x || P.y >= ls.y) return float4(c, 1.0);
	if (codeAt(P) < 0.5 || !isWorld(uv * g.xy)) return float4(c, 1.0);	// not visible ground, or the UI: no grass
	float4 gl = tex2D(low, uv);
	return float4(c * (1.0 - gl.a) + gl.rgb, 1.0);
}
#else
float4 main_grass(float2 uv : TEXCOORD0) : COLOR
{
	bool layerOut = sun.w > 0.5;	// THE MODE 1: the grass layer, not over the frame
	float3 c = tex2D(frame, uv).rgb;
	float4 none = layerOut ? float4(0, 0, 0, 0) : float4(c, 1.0);
	float2 L = (uv * g.xy - ls.zw) / g.z;	// layer px, continuous (texel i covers [i, i + 1))
	float2 P = floor(L);
	if (P.x < 0.0 || P.y < 0.0 || P.x >= ls.x || P.y >= ls.y) return none;	// off the layer (the wheel zoom's edges)
	if (tex2Dlod(blk, float4((floor(P * k.w) + 0.5) / bs.xy, 0, 0)).r < 0.5) return none;	// no blade reaches this block
	if (codeAt(P) < 0.5) return none;		// not visible ground: a blade never covers it
	if (!isWorld(uv * g.xy)) return none;	// UI over the world
	// how this pixel is lit: its screen colour against its palette colour (for roots under the UI, groundColour)
	float lit = clamp(lum(gameAt((P + 0.5) * g.z + ls.zw)) / max(lum(palAt(P)), 0.03), 0.0, 2.0);
	float t = g.w;
	float px = a.w;
	float3 acc = float3(0, 0, 0); float cover = 0.0;	// the blades laid so far: premultiplied colour, coverage (THE MODE)
	[loop] for (float dy = 0.0; dy < k.z; dy += 1.0) {
		float ry = P.y + dy;					// the row of roots tried now
		if (ry > ls.y - 0.5) break;
		// the wind at (this pixel's x, that row): its bend factor q, the tip offset at height h being q h^2
		float2 mr = float2(L.x, ry + 0.5) - a.xy;
		float gust = 0.5 + 0.5 * sin((mr.x * 0.8 + mr.y * 0.35) / 240.0 - t * 1.4);
		float q = 0.0;
		if (a.z > 0.0) {
			float swing = sin(t * 2.1 + (mr.x + mr.y * 2.0) / 140.0);
			float ripple = sin(t * 5.3 - (mr.x * 0.9 + mr.y * 0.45) / 16.0);
			q = a.z * (0.55 * gust + 0.25 * gust * swing + 0.12 * ripple * (0.3 + gust)) * k.y * k.y;
		}
		float hr = ry + 0.5 - L.y;				// this pixel's height above the row
		float x0 = floor(L.x - q * hr * hr) - 2.0;
		[loop] for (float i = 0.0; i < 5.0; i += 1.0) {
			float2 r = float2(x0 + i, ry);		// a root texel
			float code = codeAt(r);
			if (code < 1.5) continue;			// not grass
			float4 h = hash42(r - a.xy);
			if (h.x >= 0.55 * bs.w) continue;	// (cheap tests first: no root grows more often than this ...)
			float hv = ry + 0.5 + (h.z - 0.5) * 0.7 - L.y;	// this pixel's height above the root (roots jittered up / down)
			if (hv < -0.5 || hv > k.x * (0.40 + 0.60 * h.y) + 0.5) continue;	// (... nor reaches higher than this)
			float share = shareAt(r + 0.5 - a.xy);
			float grow = code < 3.5 ? smoothstep(0.15, 0.45, share) * (code < 2.5 ? 0.55 : 0.35)	// green / speck
			                        : smoothstep(0.35, 0.60, share) * 0.50;						// in a baked shadow: only in a full lawn
			if (h.x >= grow * bs.w) continue;	// no blade on this root
			float hb = k.x * (0.40 + 0.60 * h.y) * (0.80 + 0.20 * smoothstep(0.30, 0.80, share)) * (code > 2.5 && code < 3.5 ? 0.8 : 1.0);
			float lean = (frac(h.y * 13.7) - 0.5) * 0.10;
			float qb = q + a.z * 0.07 * k.y * k.y * sin(t * 7.5 + h.w * 6.2832);	// + this blade's flutter (0 when calm)
			float tip = lean * hb + qb * hb * hb;	// how far the tip leans
			float top = hb * (1.0 - 0.25 * saturate(tip * tip / (hb * hb)));	// a bent blade reaches less high
			if (hv > top + 0.5) continue;
			float hc = clamp(hv, 0.0, top);
			float s = lean + 2.0 * qb * hc;		// the blade's slope dx / dh here
			float d = abs(L.x - (r.x + 0.5 + lean * hc + qb * hc * hc)) * rsqrt(1.0 + s * s);	// distance across the blade
			float tt = hc / top;				// 0 foot .. 1 tip
			float halfw = (0.50 + 0.20 * h.y) * (1.0 - 0.80 * tt);
			float cov = saturate((halfw - d) * px + 0.5) * saturate((top - hv) * px + 0.5) * saturate(hv * px + 0.5);
			if (cov < 0.004) continue;
			// its colour: the root pixel on screen (under the UI: its palette colour, THE UI, EXACTLY) -- or, SHADOWS
			// ON THE BLADES (above), for a root in a shadow whose point here is in the light, the lit grass there
			// (never a path's colour); a lit root whose point here is in a shadow is darkened
			float2 sp = floor(float2(L.x, ry + 0.5) + hc * sun.xy);
			float sc = codeAt(sp);
			float3 rc = groundColour((code > 3.5 && sc > 1.5 && sc < 3.5) ? sp : r, lit);
			if (code < 3.5 && sc > 3.5) rc *= sun.z;
			float shade = lerp(0.70, 1.12, tt) * (0.86 + 0.28 * frac(h.w * 3.17)) + 0.18 * bs.z * gust * tt;
			acc = lerp(acc, rc * shade, cov);	// a nearer blade over the ones behind it
			cover = lerp(cover, 1.0, cov);
		}
	}
	return layerOut ? float4(acc, cover) : float4(c * (1.0 - cover) + acc, 1.0);
}
#endif
