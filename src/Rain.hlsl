// Rain.hlsl -- (new by th4) RAIN on storm nights in the normal town.
// Driver: dx_utility.cpp Rain_Present (called from PostFxPresent right after the world/UI mask and
// the mist), which decides when it shows (Rain_Active: Weather.cpp says STORM, and it is night).
// WINDOW-size pass, after the upscale: streaks are about one screen pixel wide, semi-transparent
// and anti-aliased -- not game pixels. The frame is copied to s0 and redrawn with rain over world
// pixels only. Fully procedural, nothing is stored:
//   STREAKS: three layers (far / mid / near). The screen is sheared by the wind slant and cut
//   into columns; each column carries drops at a hashed x, falling at a hashed speed, repeating
//   every 'period' pixels, and each drop of each repetition is kept or dropped by a hash against
//   the layer density -- so the pattern never visibly repeats. A pixel is lit by the drop whose
//   head is below it within 'len' pixels, brightest at the head.
//   SPLASHES: the ground is cut into cells; each cell makes a small flat ring (flattened 2:1 like
//   the isometric floor) at a hashed spot, once per hashed period, growing and fading quickly.
// All sizes are in pixels of a 1080-high window and scaled by the real window height (c1.y), so
// the rain looks the same at every resolution. WORLD-ANCHORED (c2): streaks and splashes are laid
// out in window px measured from the ground's anchor (tile 0,0 -- the Mist / Snowfall / clouds
// anchor), so a splash stays on its spot of ground and the hero walks through the rain. It used to be
// screen-fixed and players saw the splashes slide along with the camera (reported on 0.675).
//
// s0 = frame copy (point, clamp), s1 = world/UI mask from HeatHaze_BuildMask, sampled LINEAR:
//      0 = UI, >= 80/255 = world (dx_utility.cpp kOut), so m x 255/80 clamped is a soft 0..1
//      world weight (same as Mist.hlsl; a hard test would draw the mask's 2x2 steps at the orbs).
// c0 = xy window size, z rain clock in seconds (Weather.cpp, stops on pause), w strength 0..1
//      (follows the night, so rain fades in at dusk and out at dawn)
// c1 = x lightning flash 0..1 (Weather_FlashQ8: rain lights up with the strike), y window height
//      / 1080, z wind slant (x shift per y px)
// c2 = xy the ground anchor in WINDOW px (dx_utility.cpp Atmos_Anchor x window / game size)
sampler2D frame : register(s0);
sampler2D mask : register(s1);
float4 g : register(c0);
float4 k : register(c1);
float4 anc : register(c2);

float hash(float2 p)
{
	return frac(sin(dot(p, float2(127.1, 311.7))) * 43758.5453);
}

// one streak layer; sizes in window px (already scaled), speed in px/s
float rainLayer(float2 px, float t, float cellW, float period, float len, float speed, float width, float density, float seed)
{
	px.x -= px.y * k.z;								// wind slant: shear, so streaks lean
	float col = floor(px.x / cellW);
	float h1 = hash(float2(col, seed));
	float h2 = hash(float2(col, seed + 7.1));
	float h3 = hash(float2(col, seed + 3.3));
	float xC = (col + 0.2 + 0.6 * h1) * cellW;
	float lineW = saturate(1.0 - abs(px.x - xC) / width);	// ~1 px line, anti-aliased
	if (lineW <= 0.0)
		return 0.0;
	float y = h3 * period + t * speed * (0.8 + 0.4 * h2) - px.y;	// head minus pixel
	float cyc = floor(y / period);
	float s = y - cyc * period;						// how far above the head this pixel is
	if (s > len)
		return 0.0;
	float present = step(hash(float2(col + 0.5, cyc + seed * 13.0)), density);
	return lineW * present * (1.0 - s / len);
}

float splash(float2 px, float t, float sc)
{
	float2 cell = float2(46.0, 24.0) * sc;
	float2 id = floor(px / cell);
	float h = hash(id), h2 = hash(id + 17.3);
	float life = 0.30;								// s
	float per = 1.8 + 1.8 * h2;						// s between splashes of this cell
	float lt = frac((t + h * per) / per) * per;
	if (lt > life)
		return 0.0;
	float2 ctr = (id + float2(0.2 + 0.6 * h2, 0.2 + 0.6 * h)) * cell;
	float2 d = px - ctr;
	d.y *= 2.0;										// flat on the 2:1 floor
	float rad = (1.0 + 5.0 * lt / life) * sc;
	float ring = saturate(1.0 - abs(length(d) - rad) / (0.8 * sc));
	return ring * (1.0 - lt / life);
}

float4 main_rain(float2 uv : TEXCOORD0) : COLOR
{
	float3 c = tex2D(frame, uv).rgb;
	float worldW = saturate(tex2D(mask, uv).r * (255.0 / 80.0));
	if (worldW <= 0.0)
		return float4(c, 1.0);
	float2 px = uv * g.xy - anc.xy;				// window px from the ground anchor: fixed to the world
	float t = g.z, sc = k.y;
	float r = 0.0;
	//                          cellW      period      len        speed       width       density seed
	r += 0.20 * rainLayer(px, t,  9.0 * sc, 260.0 * sc, 22.0 * sc,  900.0 * sc, 0.8 * sc, 0.55, 1.0);
	r += 0.28 * rainLayer(px, t, 14.0 * sc, 340.0 * sc, 34.0 * sc, 1300.0 * sc, 0.9 * sc, 0.50, 2.0);
	r += 0.36 * rainLayer(px, t, 23.0 * sc, 480.0 * sc, 55.0 * sc, 1800.0 * sc, 1.1 * sc, 0.45, 3.0);
	r += 0.30 * splash(px, t, sc);
	float3 rainCol = lerp(float3(0.62, 0.68, 0.78), float3(1.0, 1.0, 1.0), k.x * 0.6);
	float amount = saturate(r) * g.w * (1.0 + k.x) * worldW;
	return float4(lerp(c, rainCol, saturate(amount)), 1.0);
}
