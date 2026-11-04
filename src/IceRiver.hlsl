// IceRiver.hlsl -- (new by th4) ICE RIVER: the river of Ice Age Tristram as a still, shiny MIRROR of a winter sky by day.
// Driver: dx_utility.cpp IceRiver_Present (WHEN, the clock, the parallax, the storm and dusk numbers); the map of which
// pixels are water: dx_utility.cpp IceRiver_BuildMask, from TownWater.cpp's river mask (TownWater_MirrorRows).
// Mordor's references: a winter river under a blue sky with white cumulus, the snowy trees and banks mirrored; seen from
// above (our camera) the water itself is deep blue in the middle and lighter toward the shallow banks. An earlier "mirror
// ice" (an overly blue tint with blue stripes) was rejected as "a disaster": nothing here TINTS the water -- every colour
// comes from the mirrored sky and clouds over a dark water body.
// THE LOOK, per water pixel:
//   SKY      a gradient from pale blue-white (top of the screen = far water mirrors the horizon) to deep blue (bottom =
//            near water mirrors the high sky);
//   CLOUDS   soft cumulus from a tileable noise texture (s3, built on the CPU): domain-warped, stretched sideways
//            (seen at a slant), lit from below on screen (a mirror turns the sky upside down: the sunlit tops of the clouds
//            face the bottom of the screen); they drift on the wind (dx_utility.cpp ICE_CLOUD_WIND, faster in a storm) and
//            move only ICE_SKY_PARALLAX as fast as the ground
//            when the view scrolls -- THE DEPTH: the water slides past a far-away sky, as in a real reflection;
//   STORM    c1.w: the sky turns a paler, softer blue under more cloud (Snowfall.cpp's calm / storm spells, eased like
//            the snow) -- NO GREY anywhere in the sky (Mordor, 2026-10-10: "grey has to go"; the first look had a near-white
//            horizon, a grey overcast and grey-blue cloud bellies, and on the water all three read as grey);
//   FRESNEL  the water mirrors more of the sky at the top of the screen (far water, seen at a slant) than at the bottom
//            (near water, seen from above, where the dark water body shows): bright far, deep navy near;
//   DEPTH    the distance to the bank (a search in the map, s2): the water body is darkest in the middle and lighter
//            in the shallows, with a thin dark line at the very edge;
//   ART      the river art's own light / dark bends the mirror image by a pixel or two (a slightly uneven surface: the
//            sky shimmers over it when the view moves) and keeps a faint trace of its texture -- never a flat decal;
//   SOLID    the river art is speckled with grains and checkerboards of other entries; TownWater.cpp fills them into
//            its mask while the mirror shows (MirrorFill), so they arrive here as water;
//   HEROES   their reflections (TownWater.cpp DrawReflection: 8-bit, darkened, thinned out toward the head by a 4 x 4
//            ordered dither) lie over the sky as real see-through: the dither is averaged back into an opacity.
// Snowflakes, sprites, HP bars and the UI are never painted (code 0 in the map), so the snow falls over the mirror.
//
// s0 = frame copy (point, clamp), window resolution
// s2 = THE MAP (point; game resolution, screen-parallel, L8): 0 = covered (sprite, flake, UI) -- never painted, counts
//      as water for the bank search; 24 = bank (terrain that is not river water); 128 = water; 192 = water with a hero
//      reflection pixel (dx_utility.cpp IceRiver_BuildMask)
// s3 = cloud noise (linear, WRAP; 256 x 256 L8, tileable fBm)
// c0 = xy game resolution, z THE VIEW's zoom (game px on screen per world px), w clock in seconds (stops on pause; unused)
// c1 = xy the sky's offset (world px): the parallax + the clouds' drift (summed on the CPU, ICE_CLOUD_WIND), z strength 0..1
//      (dusk fade; 0 at night), w storm 0..1
// c2 = x brightness (dims over the dusk), yzw unused
sampler2D frame : register(s0);
sampler2D map : register(s2);
sampler2D noise : register(s3);
float4 g : register(c0);
float4 sky : register(c1);
float4 lit : register(c2);

// ---- the look (tunable) -----------------------------------------------------------------------------------------
// NO GREY (STORM above): every sky colour here is a blue; only the lit cloud tops are white
static const float3 SKY_HORIZON   = float3(0.58, 0.76, 0.96);	// clear light blue far sky (top of the screen); was a near-white
static const float3 SKY_ZENITH    = float3(0.20, 0.42, 0.76);	// deep blue high sky (bottom of the screen)
static const float3 CLOUD_LIT     = float3(1.00, 1.00, 1.00);
static const float3 CLOUD_SHADE   = float3(0.62, 0.74, 0.92);	// soft blue cloud bellies; was grey-blue
static const float3 OVERCAST_LIT  = float3(0.68, 0.80, 0.94);	// storm sky: pale soft blue; was white-grey
static const float3 OVERCAST_DARK = float3(0.46, 0.60, 0.80);	// ... its denser cloud; was grey
static const float3 BODY_DEEP     = float3(0.030, 0.075, 0.115);	// the water itself, mid-stream
static const float3 BODY_SHALLOW  = float3(0.18, 0.34, 0.50);		// ... in the shallows by the bank; was grey-teal
static const float  REFLECT_FAR   = 0.80;	// how much of the sky the water mirrors at the top of the screen (far water,
static const float  REFLECT_NEAR  = 0.45;	// seen at a slant) .. at the bottom (near water, seen from above: more of the
											// dark water body shows -- FRESNEL, as in the photos: bright far, deep navy near)
static const float  REFLECT_SHALLOW = 0.80;	// x that in the shallows (more of the bed shows through)
static const float  REFLECT_CLOUD = 0.95;	// a white cloud mirrors up to this much (x CLOUD_BRIGHT of its cover): mixed with
static const float  CLOUD_BRIGHT  = 0.70;	// the dark water like the sky, white clouds came out light GREY on the water
static const float  DEPTH_FULL    = 9.0;	// world px from the bank where the water is fully deep
static const float  SHORE_DARK    = 0.40;	// the dark line at the very edge (0 = none)
static const float  CLOUD_SCALE   = 900.0;	// world px per noise tile
static const float  CLOUD_SQUASH  = 1.7;	// clouds this many times wider than tall
static const float  CLOUD_COVER   = 0.53;	// noise level where a cloud begins (calm); lower = more cloud
static const float  CLOUD_SOFT    = 0.17;	// how soft their edges are
static const float  WOBBLE_PX     = 45.0;	// sky offset (world px) per unit of art luminance slope
static const float  ART_TRACE     = 0.8;	// how much of the art's fine light / dark is kept
static const float  REFL_OPACITY  = 0.92;	// hero reflections over the sky

static const float3 LUMW = float3(0.299, 0.587, 0.114);

// the map at uv + off game px, the offset ROUNDED to whole game px: point samples never land on a texel edge
static float2 PX;	// one game px in uv (set first thing in main)
float codeAt(float2 uv, float2 off) { return tex2Dlod(map, float4(uv + floor(off + 0.5) * PX, 0, 0)).r * 255.0; }
float lumAt(float2 uv)  { return dot(tex2Dlod(frame, float4(uv, 0, 0)).rgb, LUMW); }
bool isBank(float v) { return abs(v - 24.0) < 8.0; }
// direction k of 8 on a WORLD circle: the floor is 2:1 on screen, so a world px up the screen is half a game px
float2 dirOf(float k) { float sn, cs; sincos(k * 0.785398, sn, cs); return float2(cs, 0.5 * sn); }

float4 main_iceriver(float2 uv : TEXCOORD0) : COLOR
{
	PX = 1.0 / g.xy;
	float3 c = tex2Dlod(frame, float4(uv, 0, 0)).rgb;
	float code = codeAt(uv, 0);
	if (code < 96.0 || sky.z <= 0.0)
		return float4(c, 1.0);					// covered (a sprite, a flake, the UI) or the bank: most of the screen
	float vz = g.z;

	// DEPTH: the nearest bank along 8 rays (r = 1, 2.5, 4 ... 13 world px). A bank needs TWO bank taps in a row, so a
	// stray grain the fill missed is not taken for the bank: depth = the first of the two, - 0.5
	float depth = DEPTH_FULL;
	[loop] for (int d = 0; d < 8; ++d) {
		float2 dv = dirOf(d) * vz;
		bool prev = false;
		[loop] for (int s = 0; s < 9; ++s) {
			float r = 1.0 + 1.5 * s;
			if (r - 1.5 > depth) break;
			bool bank = isBank(codeAt(uv, dv * r));
			if (bank && prev) { depth = min(depth, r - 2.0); break; }
			prev = bank;
		}
	}
	depth = max(depth, 0.5);
	float deep = smoothstep(0.0, DEPTH_FULL, depth);

	// ART: slope and fine detail of the river art's light (reflection pixels and the bank around carry other colours)
	float lc = dot(c, LUMW);
	float lL = lumAt(uv - float2(PX.x, 0)), lR = lumAt(uv + float2(PX.x, 0));
	float lU = lumAt(uv - float2(0, PX.y)), lD = lumAt(uv + float2(0, PX.y));
	float2 slope = clamp(float2(lR - lL, lD - lU), -0.06, 0.06);
	float detail = clamp(lc - 0.25 * (lL + lR + lU + lD), -0.04, 0.04);
	if (code > 160.0) { slope = 0; detail = 0; }

	// SKY + CLOUDS, in sky coordinates: centre-relative world px + the parallax offset, bent by the art
	float2 p = (uv * g.xy - 0.5 * g.xy) / vz + sky.xy + slope * WOBBLE_PX;
	float2 q = p / CLOUD_SCALE;
	q.y *= CLOUD_SQUASH;
	q += (tex2Dlod(noise, float4(q * 0.43 + 0.31, 0, 0)).r - 0.5) * 0.30;	// domain warp: billowy, not blobby
	float n1 = tex2Dlod(noise, float4(q, 0, 0)).r;
	float n2 = tex2Dlod(noise, float4(q * 2.3 + 0.17, 0, 0)).r;
	float dens = n1 * 0.68 + n2 * 0.32;
	float2 ql = q + float2(0.004, 0.030);						// toward the light (down on screen: the mirror)
	float densL = tex2Dlod(noise, float4(ql, 0, 0)).r * 0.68 + tex2Dlod(noise, float4(ql * 2.3 + 0.17, 0, 0)).r * 0.32;
	float cover = CLOUD_COVER - 0.20 * sky.w;
	float cloud = smoothstep(cover, cover + CLOUD_SOFT, dens);
	float shade = saturate((densL - dens) * 5.0 + 0.15);
	float3 cl = lerp(CLOUD_LIT, CLOUD_SHADE, shade);
	float3 s = lerp(SKY_HORIZON, SKY_ZENITH, pow(saturate(uv.y), 0.85));
	s = lerp(s, cl, cloud);
	float3 storm = lerp(OVERCAST_LIT, OVERCAST_DARK, saturate((dens - 0.35) * 2.2));
	s = lerp(s, storm, sky.w);
	float bright = cloud * (1.0 - shade) * (1.0 - sky.w);	// a lit white cloud top (not in a storm's soft blue)

	// the WATER: body (deep .. shallow) + the mirrored sky, the art's trace, the dark line at the edge
	float3 body = lerp(BODY_SHALLOW, BODY_DEEP, deep);
	float f = lerp(REFLECT_FAR, REFLECT_NEAR, saturate(uv.y)) * lerp(REFLECT_SHALLOW, 1.0, deep);
	f = lerp(f, REFLECT_CLOUD, bright * CLOUD_BRIGHT);	// white clouds stay white (REFLECT_CLOUD)
	float3 w = lerp(body, s, f);
	w += detail * ART_TRACE;
	w *= 1.0 - SHORE_DARK * (1.0 - smoothstep(0.5, 2.0, depth));
	w *= lit.x;

	// HEROES: the 4 x 4 ordered dither back into an opacity (any 4 x 4 window holds each threshold once)
	float rn = 0.0; float3 rc = 0;
	[loop] for (int j = 0; j < 4; ++j)
		[loop] for (int i = 0; i < 4; ++i) {
			float2 t = uv + floor(float2(i - 2, j - 2) * vz + 0.5) * PX;
			if (codeAt(t, 0) > 160.0) { rn += 1.0; rc += tex2Dlod(frame, float4(t, 0, 0)).rgb; }
		}
	if (rn > 0.0) w = lerp(w, rc / rn, rn / 16.0 * REFL_OPACITY);

	return float4(lerp(c, saturate(w), sky.z), 1.0);
}
