// River.hlsl -- (new by th4) THE RUNNING RIVER of Tristram (config WaterShimmer, the same switch as the ripple).
// Driver: dx_utility.cpp River_Present (RIVER FLOW there). The direction of the stream comes from TownWater.cpp
// BuildRiverFlow (THE FLOW MAP: every stream runs to its LEFT end, along its own bends -- Mordor: the river flows
// south-west). Window size, one pass, run where PostFx.hlsl's water ripple used to act (after the retro modes,
// before the PostFx.hlsl pass), which then leaves the town river alone. The cave water maps and Ravenholm's
// STAGNANT water keep PostFx.hlsl's ripple in place (Mordor: the river "ripples like a swamp" -- that ripple is
// right for still water, not for a river).
//
// WHAT IT DOES to a pixel of the river (mask code 170):
//   THE SLIDE: the water's own texture moves downstream. The art is not tileable and cannot scroll forever, so it
//   is the usual flow-map trick: two copies of the water, each pulled from upstream by an offset that grows over
//   one cycle (c3.z world px in 1 / c2.w s, i.e. at c2.z px/s) and then jumps back, half a cycle apart, each
//   weighted 0 at its own jump -- so no jump is ever seen. The whole river is in the same phase: a phase that
//   drifted over the map (to keep it from pulsing in step) made the cross-fade between the copies sweep over the
//   water at 70+ px/s in a direction of its own -- it read as water running sideways (Mordor, near the cows). A short
//   cycle (dx_utility.cpp RIVER_CYCLE_SEC) keeps the in-step pulse faint instead. A copy that would come from
//   outside the moving water (a bank, a reflection, a sprite) stays where it is instead.
//   THE RIPPLE travels with the water: waves across the stream, moving downstream at the same speed. Their fronts
//   are lines of equal STREAM PHASE -- how far downstream a spot lies, worked out on the CPU (TownWater.cpp THE
//   STREAM PHASE) and stored in the flow map's b/a as a wrapped angle, one turn per RIVER_WAVE_PERIOD world px; each
//   wave is a whole number of waves per turn (c6), so the wrap never shows. Do NOT go back to dot(position, direction):
//   that moves along the direction only where the direction is the same all around; in the bends the position term
//   took over and the ripples ran sideways and backwards (on ~40 % of the lower river). The time phases (c5) come
//   wrapped from the CPU. A faint world-fixed term (bend) keeps the fronts from being ruler-straight.
// A pixel with a hero REFLECTION in its mask texel (code 150, dx_utility.cpp HeatHaze_BuildMask, TownWater.cpp
// RiverReflPix) does not slide (that would drag the reflection downstream and double it) and does not take the
// stream's ripple either: on running water a reflection stays where it is and the moving surface breaks it into
// rows that shiver sideways -- a sideways-only sway per world row, fast. The water around it never samples it
// (the ripple and the slide take only code 170), so the reflection does not smear into the stream.
// Where the flow map holds no flow (the two small pieces of river tiles elsewhere in town) the water and its
// reflections ripple in place as before.
// THE MAP POSITION of a pixel is found as PostFx.hlsl finds its water's: the DYNAMIC ZOOM and the Perspective
// warp taken off first, then measured from the centre of map cell (0,0) (TownWater.cpp THE MAP ANCHOR).
//
// s0 = frame copy (LINEAR: the offsets are sub-pixel), s1 = world/UI mask (POINT -- its codes must stay exact:
//      170 water, 150 water with a reflection, 80 world, 0 UI), s2 = THE FLOW MAP, 112 x 112 (u = row, v = col),
//      LINEAR: rg = screen direction (0.5 = 0), shrinking toward the banks; ba = the stream phase as cos / sin
// c0 = xy game resolution, z 1 / the DYNAMIC ZOOM factor, w clock in s (stops on pause)
// c1 = xy the DYNAMIC ZOOM shift (game px), z Alt+Z factor (1 or 2), w ripple sway in window px (WaterShimmer)
// c2 = xy centre of map cell (0,0) in DRAWN game px, z speed (world px / s), w cycles per second
// c3 = xy window px per world px, z slide length per cycle (world px), w unused
// c4 = xy Perspective coefficients a / b (0 = off), zw 1 / window size
// c5 = xyz the three ripple waves' time phases (rad, 0..2 pi), w unused
// c6 = xyz the three ripple waves per turn of the stream phase (dx_utility.cpp RIVER_WAVES), w unused
sampler2D frame : register(s0);
sampler2D heat : register(s1);
sampler2D flow : register(s2);
float4 g : register(c0);
float4 v : register(c1);
float4 a : register(c2);
float4 s : register(c3);
float4 pp : register(c4);
float4 wp : register(c5);
float4 wk : register(c6);

float Mask(float2 uv) { return tex2Dlod(heat, float4(uv, 0, 0)).r; }
bool isWater(float m) { return m > 0.5 && m < 0.85; }		// 170 or 150
bool isFlowing(float m) { return m > 0.62 && m < 0.85; }	// 170 only

float4 main_river(float2 uv : TEXCOORD0) : COLOR
{
	float3 c = tex2D(frame, uv).rgb;
	float here = Mask(uv);
	if (!isWater(here)) return float4(c, 1.0);
	// the map position: off with the DYNAMIC ZOOM, then the Perspective warp (PostFx.hlsl water shimmer), then from cell (0,0)
	float2 p = (uv * g.xy - v.xy) * g.z;
	if (pp.x > 0.0 || pp.y > 0.0) {
		float u = p.y / g.y;
		p.y = g.y * (u + pp.x * u * (1.0 - u));
		p.x = g.x * 0.5 + (p.x - g.x * 0.5) / (1.0 + pp.y * u);
	}
	float2 M = (p - a.xy) / v.z;			// world px from the centre of map cell (0,0), screen axes
	float2 rc = float2(M.x / 32.0 + M.y / 16.0 + 1.0, M.y / 16.0 + 1.0 - M.x / 32.0) * 0.5;
	float4 fm = tex2Dlod(flow, float4(rc / 112.0, 0, 0));
	float2 f = fm.rg * 2.0 - 1.0;			// the stream: screen direction x its strength
	float fl = length(f);
	float t = g.w;
	if (fl <= 0.05) {						// no flow here: PostFx.hlsl's ripple in place, water and reflections alike
		float2 rip;
		rip.x = sin(M.y * 0.21 + t * 1.7) * 0.6 + sin((M.x * 0.5 + M.y) * 0.09 - t * 1.1) * 0.4;
		rip.y = sin(M.x * 0.11 + M.y * 0.07 + t * 1.3) * 0.5;
		float2 ruv = uv + rip * v.w * pp.zw;
		if (!isWater(Mask(ruv))) ruv = uv;	// only sample water from water: the banks stay clean
		return float4(tex2Dlod(frame, float4(ruv, 0, 0)).rgb, 1.0);
	}
	if (!isFlowing(here)) {					// a REFLECTION on running water: rows shivering sideways, in place
		float dx = sin(M.y * 0.9 + t * 7.0) * 0.6 + sin(M.y * 0.37 - t * 4.1) * 0.4;
		float2 ruv = uv + float2(dx * v.w * pp.z, 0.0);
		if (!isWater(Mask(ruv))) ruv = uv;
		return float4(tex2Dlod(frame, float4(ruv, 0, 0)).rgb, 1.0);
	}
	// THE RIPPLE (window px): waves across the stream, along the stored stream phase
	float2 q = fm.ba * 2.0 - 1.0;
	float th = atan2(q.y, q.x);				// the stream phase: one turn per RIVER_WAVE_PERIOD world px downstream
	float bend = sin(M.x * 0.023 + M.y * 0.031) * 0.6;
	float2 rip;
	rip.x = sin(th * wk.x - wp.x + bend) * 0.6 + sin(th * wk.y - wp.y + 1.7 - bend * 0.8) * 0.4;
	rip.y = sin(th * wk.z - wp.z + 0.8 + bend) * 0.5;
	float2 r = rip * v.w * pp.zw;
	// THE SLIDE: two copies, half a cycle apart, each from upstream; only moving water is taken (never a bank or a reflection)
	float ph0 = frac(t * a.w), ph1 = frac(ph0 + 0.5);	// one phase for the whole river (see THE SLIDE above)
	float w0 = 1.0 - abs(2.0 * ph0 - 1.0);	// 0 when copy 0 jumps back, 1 half a cycle later
	float2 k = f * s.z * s.xy * pp.zw;		// one cycle's offset, in uv
	float2 uA = uv + r - k * (ph0 - 0.5), uB = uv + r - k * (ph1 - 0.5);
	if (!isFlowing(Mask(uA))) uA = uv;
	if (!isFlowing(Mask(uB))) uB = uv;
	float3 col = tex2Dlod(frame, float4(uA, 0, 0)).rgb * w0 + tex2Dlod(frame, float4(uB, 0, 0)).rgb * (1.0 - w0);
	return float4(col, 1.0);
}
