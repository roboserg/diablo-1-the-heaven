// Frost.hlsl -- (new by th4) ICE FROST creeping in from the screen edges (Ice Age, Glacial Caverns,
// Frozen Loch). Driver: dx_utility.cpp Frost_Present (called from PostFxPresent right after the mist /
// rain / glow passes), which decides when it shows (Frost_Wanted), how far the frost has grown
// (Frost_Update) and owns the FROST FEATHERS texture (generated once on the CPU, FrostTex_Generate).
// Window size, one pass: the frame is copied to s0 and redrawn with frost over WORLD pixels only --
// panels, text and the cursor stay clear (same soft mask test as Mist.hlsl).
//
// The feather texture is in EDGE SPACE (x along an edge, y = depth in from it); it is laid along all
// FOUR screen edges (the world shows beside and above the bottom panel; the mask keeps the panel
// itself clear), each at its own offset along the edge so they do not repeat each other; the corners
// get both of their edges -- denser there, as on a real frozen pane. Channels: r = ice line
// brightness, g = when the feather GROWS there (share of the texture depth), b = film between the
// feathers, a = when the film grows there. A texel shows once the growth front has passed its time,
// so the frost grows along the feathers as it creeps.
// THE GROWTH FRONT IS AN OVAL: the clear middle is an ellipse shaped like the screen, radius c0.x in
// normalised screen coords (1 = touching the edge midpoints, sqrt(2) = the corners), shrinking as the
// frost creeps in -- so frost starts in the corners, spreads along the edges and closes in as an
// oval, not as a rectangle. For each edge the local front is the distance from that edge to the oval
// at this point (front()), at least the thin edge crust c3.x. The radius wobbles a little with the
// angle, so the clear oval is not a perfect ellipse.
// Frost is on the "glass": screen space, it does not move with the ground when the hero walks.
//
// s0 = frame copy (linear, clamp), s1 = world/UI mask from HeatHaze_BuildMask (linear;
//      0 = UI, >= 80/255 = world; m / (80/255) clamped = soft 0..1 world weight, see Mist.hlsl)
// s2 = feather texture FROST_TEX_W x FROST_TEX_H (linear, u WRAP, v CLAMP)
// c0 = x radius of the clear oval (see above), y clock in seconds (stops on pause),
//      z window px per texel, w ice opacity
// c1 = xy 1 / window size, zw window size in px
// c2 = xy 1 / (FROST_TEX_W, FROST_TEX_H), zw (FROST_TEX_W, FROST_TEX_H)
// c3 = x growth front of the thin crust along every edge (share of the texture depth), yzw unused
sampler2D frame : register(s0);
sampler2D mask : register(s1);
sampler2D feathers : register(s2);
float4 f0 : register(c0);
float4 f1 : register(c1);
float4 f2 : register(c2);
float4 f3 : register(c3);

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

// growth front for one edge, in shares of the texture depth: the distance from the edge to the oval
// of radius R along this row / column. across = the other normalised coordinate (-1..1), size = the
// window size across the edge in px. Past the oval's end (corners) the whole row is frost.
float front(float R, float across, float size)
{
	float q = R * R - across * across;
	float d = (q > 0.0 ? 1.0 - sqrt(q) : 1.0) * 0.5 * size;
	return max(d / (f0.z * f2.w), f3.x);
}

// one edge: u = window px along the edge (plus an offset), v = window px in from it, g = its front.
// Adds this edge's grown ice line and film into ink / film (max: overlapping frost does not add up).
void edge(float u, float v, float g, inout float ink, inout float film)
{
	float2 t = float2(u, v) / f0.z;			// texels
	if (t.y >= f2.w - 1.0)
		return;
	float4 s = tex2D(feathers, t * f2.xy);
	ink  = max(ink,  s.r * saturate((g - s.g) / 0.03));	// 1 once the front has passed the feather's time
	film = max(film, s.b * saturate((g - s.a) / 0.03));
}

float4 main_frost(float2 uv : TEXCOORD0) : COLOR
{
	float3 c = tex2D(frame, uv).rgb;
	float worldW = saturate(tex2D(mask, uv).r * (255.0 / 80.0));	// 0 = UI, text, cursor; 1 = world
	if (worldW <= 0.0 || f0.x <= 0.0)
		return float4(c, 1.0);
	float2 px = uv * f1.zw;
	float2 n = uv * 2.0 - 1.0;						// normalised screen coords, -1..1
	float ang = atan2(n.y, n.x);
	float R = f0.x * (1.0 + 0.05 * sin(5.0 * ang + 1.3) + 0.03 * sin(9.0 * ang + 0.7) + 0.02 * sin(14.0 * ang + 2.1));
	float gx = front(R, n.y, f1.z), gy = front(R, n.x, f1.w);
	float ink = 0.0, film = 0.0;
	edge(px.y + 3071.0, px.x, gx, ink, film);					// left
	edge(px.y + 7919.0, f1.z - 1.0 - px.x, gx, ink, film);		// right
	edge(px.x + 1237.0, px.y, gy, ink, film);					// top
	edge(px.x + 5501.0, f1.w - 1.0 - px.y, gy, ink, film);		// bottom (the mask keeps the panel clear)
	float cover = saturate(ink + film);
	if (cover <= 0.0)
		return float4(c, 1.0);

	// frosted glass: the world under the ice is refracted a little and cooled
	float2 off = float2(hash(floor(px * 0.5)) - 0.5, hash(floor(px * 0.5) + 31.0) - 0.5) * 3.0 * f1.xy * cover;
	float3 under = tex2D(frame, uv + off).rgb;
	float lum = dot(under, float3(0.299, 0.587, 0.114));
	under = lerp(under, lum * float3(0.80, 0.92, 1.10), 0.55 * cover);

	// a few slow glints on the ice lines
	float2 gc = floor(px / 3.0);
	float gp = hash(gc);
	float glint = step(0.993, gp) * pow(saturate(sin(f0.y * (0.6 + gp) + gp * 40.0)), 24.0) * ink;

	const float3 ICE = float3(0.82, 0.91, 1.00);
	float a = saturate(ink * 0.9 + film * 0.45) * f0.w;
	float3 frosted = lerp(under, ICE, a) + glint * 0.6;
	return float4(lerp(c, saturate(frosted), worldW), 1.0);
}
