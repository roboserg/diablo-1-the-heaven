// SnowDrift.hlsl -- (new by th4) DRIFTING SNOW: in an Ice Age STORM, clouds of snow blown along the ground (Mordor,
// 2026-10-11, after a blizzard video: "the snow advection effect above ground ... during storms"). Driver and numbers:
// dx_utility.cpp SnowDrift_Present (DRIFTING SNOW), in PostFxPresent after the flame halos and before the breath and the
// falling snow (SnowFlakes.hlsl), which lie on top of it.
// Window size, one pass, NO frame copy: written as colour x thickness and blended SCREEN (frame + drift x ( 1 - frame ),
// like the falling snow): only ever brighter -- a cloud washes out the shadows, dark walls and trees under it and hardly
// shows on the white snow, as real drifting snow does, and it never greys anything.
// FIXED TO THE GROUND (Mordor's choice; the falling snow is fixed to the screen): the pixel's world position from the
// tile (0,0) anchor and the zoom, as Mist.hlsl, the vertical axis doubled so the clouds lie flat on the 2:1 floor.
// THE WIND: each octave slides to the right (with the flakes) by its share of the wind's travel (Snowfall.cpp
// Snowfall_WindTravel: the sum of THE SPELLS' wind, so it speeds up and slows down with the storm and stops on pause) --
// the big puffs slowest, the fine detail fastest -- and a slow, large warp bends them, so the puffs churn and change shape
// as they go instead of sliding past like a picture. Every cell is stretched along the wind (the CPU's scales).
// The CPU sends each octave's offset wrapped where the noise repeats exactly, so no precision is lost however long the storm
// blows, and its 1 / scale (one place for the numbers: dx_utility.cpp kDriftOctaves).
// THE TURNED GRIDS: value noise on a square grid shows its grid -- boxy puffs with level and upright edges, which slide
// past as rectangles. The big, middle and fine octaves are each turned by a 3-4-5 angle (ROT_*, three different ones), so
// no two share a grid: an offset of 5 x 289 cells along the wind then lands on whole multiples of 289 (e.g. ( 4, 3 ) x 289),
// where hash() repeats -- those three are wrapped at 1445, the unturned warp at 289.
// s0 = world/UI mask from HeatHaze_BuildMask, sampled LINEAR: 0 = UI, >= 80/255 = world (as Mist.hlsl)
// s1 = the FULL-RES WORLD MAP (POINT; 1 = world, 0 = UI), used inside the c5 box only (the town dialog / speech box, as
//      SnowFlakes.hlsl: the half-res mask calls the whole box UI)
// c0 = xy game resolution, z zoom (View_Z), w -
// c1 = xy anchor (game px, the screen position of tile 0,0), z thickness 0..1 (Snowfall_Storm x DRIFT_ALPHA),
//      w where the clouds begin on the noise (DRIFT_COVER: lower = more cloud, fewer gaps)
// c2 = rgb colour (day .. night)
// c3 = xy the big puffs' offset, zw the warp's (cells, wrapped at 289)
// c4 = xy the middle octave's offset, zw the fine octave's
// c5 = the see-through box in screen uv (x0, y0, x1, y1; all 2 = none)
// c6 = xy the big puffs' 1 / scale (cells per world px, x along the wind, y across), zw the warp's
// c7 = xy the middle octave's 1 / scale, zw the fine octave's
sampler2D mask : register(s0);
sampler2D world : register(s1);
float4 g : register(c0);
float4 a : register(c1);
float4 col : register(c2);
float4 off0 : register(c3);
float4 off1 : register(c4);
float4 seeBox : register(c5);
float4 sc0 : register(c6);
float4 sc1 : register(c7);

// (th4) HASH WITHOUT SINE and value noise: copied from Mist.hlsl (the same function is in CloudShadow / Frost /
// TreeSway.hlsl; Mist.hlsl has the why) -- keep them alike. Repeats every 289 cells (the wrap below).
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

static const float2x2 ROT_BIG  = float2x2(0.8, -0.6, 0.6, 0.8);	// THE TURNED GRIDS (above)
static const float2x2 ROT_MID  = float2x2(0.8, 0.6, -0.6, 0.8);
static const float2x2 ROT_FINE = float2x2(0.6, -0.8, 0.8, 0.6);

float4 main_snowdrift(float2 uv : TEXCOORD0) : COLOR
{
	float w = saturate(tex2D(mask, uv).r * (255.0 / 80.0));	// 0 = UI, text, cursor; 1 = world
	if (all(uv >= seeBox.xy) && all(uv < seeBox.zw))
		w = tex2Dlod(world, float4(uv, 0, 0)).r;
	if (w <= 0.0)
		return float4(0, 0, 0, 1);		// SCREEN with black = the frame as it is
	float2 wp = (uv * g.xy - a.xy) / g.z;	// 1x px from tile (0,0)
	float2 ground = float2(wp.x, wp.y * 2.0);
	float2 q = ground * sc0.zw - off0.zw;
	float2 warp = float2(vnoise(q), vnoise(q + 37.0)) - 0.5;	// +-0.5 cell of the warp; 37 = whole, keeps the 289 repeat
	float big  = vnoise(mul(ROT_BIG, ground * sc0.xy - off0.xy) + warp * 1.2);
	float mid  = vnoise(mul(ROT_MID, ground * sc1.xy - off1.xy) + warp * 0.6);
	float fine = vnoise(mul(ROT_FINE, ground * sc1.zw - off1.zw));
	float n = big * 0.58 + mid * 0.28 + fine * 0.14;
	float d = smoothstep(a.w, a.w + 0.38, n);	// clouds with gaps between them, soft edges
	return float4(col.rgb * (d * a.z * w), 1.0);
}
