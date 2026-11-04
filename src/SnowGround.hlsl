// SnowGround.hlsl -- (new by th4) SNOW GROUND: the open ground of Ice Age Tristram under a blanket of bright snow.
// Driver: dx_utility.cpp SnowGround_Present (WHEN, the numbers, why a true-colour pass: the Ice Age town palette is
// dark). Which pixels: TownWater.cpp SNOW GROUND -- a pre-zoom layer holding, for each pixel of visible open ground
// (walkable and tree cells' floors that nothing covered later), the palette entry the ART put there; 0 elsewhere
// (houses, walls, fences, rocks, the river, sprites, items keep their own look).
// THE LOOK: one even white over the whole ground -- paths and grass vanish under it -- except the art's BAKED SHADOWS
// (its darkest ground entries, art luminance below c2.w..c3.w), which stay as blue shadows on the snow, plus a little
// of the art's own light / dark (c4) so the snow is not flat. The snow takes the LIGHT the game gave the pixel: the
// finished colour's luminance over the art entry's own (the day / night palette, torches, Enriched Light) -- dim at
// night, bright by day, lit around the hero.
// World pixels only (world/UI mask). Follows THE VIEW / the DYNAMIC ZOOM through c0 like SunRays.hlsl's ground layer.
//
// s0 = frame copy (point), s1 = world/UI mask (linear; m / (80/255) = 0..1 world weight)
// s2 = the ground layer (pre-zoom, POINT): the art's palette entry / 255, 0 = not open ground
// s3 = 256 x 2 (POINT): row 0 each palette entry's art luminance (FilePalette, 0.299 / 0.587 / 0.114); row 1 its luminance
//      in the CURRENT palette (day / night, gamma, contrast) -- the cap that keeps a snowflake on the snow from turning
//      white (dx_utility.cpp FLAKES ON THE SNOW)
// c0 = xy screen uv -> layer px, zw the DYNAMIC ZOOM offset
// c1 = xy 1 / layer size, z strength 0..1, w the light's cap
// c2 = rgb lit snow, w art luminance from which the ground is fully lit
// c3 = rgb snow in the art's baked shadows, w art luminance below which it is fully shadow
// c4 = x how much of the art's own light / dark is kept, y its art luminance low end, z 1 / its range, w the flake cap
//      (x the entry's own light, row 1 / row 0)
// c2 / c3 / c4.x move to the moonlit night colours by night (dx_utility.cpp ICE AGE NIGHT)
// s4 = the FULL-RES WORLD MAP (POINT; 1 = world, 0 = UI), used inside the c5 box only
// c5 = the see-through dialog box in screen uv (x0, y0, x1, y1; all 2 = none): its 50 % dither defeats the half-res
//      mask (every 2 x 2 holds a dot), so the world pixels between the dots are found in s4 (dx_utility.cpp SnowGround_Present)
// The frozen river's water is never in the layer: it keeps the 8-bit look it had before the snow (Mordor: a blue
// "mirror" ice drawn here was "a disaster" and was removed).
// THE LIGHT (main_snowground) -- KEEP IT PER PALETTE ENTRY (Mordor, 2026-10-10, after three bad rounds): the light the
// game gave a pixel is its finished luminance over its art entry's, capped at lay.w AND at the entry's own light in the
// CURRENT palette (s3 row 1) x tx.w. That second cap is what makes the night look: one light per palette ENTRY, so the
// snow is even and a snowflake drawn over it (the layer was recorded before the flakes) takes the ground's light. It was
// removed once on a wrong theory (black dots -- they were holes in the layer, TownWater.cpp WATER SPECKS ON THE BANK)
// and the snow broke twice: (1) the light measured per WINDOW pixel alone draws bright rings on every game pixel's edge
// at night (the window frame is the game frame stretched, blended at the edges; by day it all clips to white and
// hides); (2) capping each pixel at its neighbours' light drew a grid over all the snow under the zoom. NEVER take the
// palette cap out, never measure the light from window-pixel neighbours. The frame is brighter than ScreenPalette, so the
// cap acts on nearly all the snow: it IS the snow's light (by day it is above lay.w, so the day look is unchanged).
// NO BLACK DOTS (Mordor: "leave a trace so YOU DON'T DO THAT AGAIN"): dark dots in the banks' shadows were HOLES in the
// layer (TownWater.cpp WATER SPECKS ON THE BANK) -- found with a debug view of 3 x 3 markers per cause, after two guessed
// fixes failed. For dots in the snow: find WHICH pixels they are first.
// c6 = xy the ground anchor in layer px (tile 0,0: dx_utility.cpp Atmos_AnchorDrawn / the Alt+Z zoom), z the clock in
//      seconds (stops on pause), w the night 0..1 (Snowfall_NightQ8) -- MOONLIGHT GLINTS below
// MOONLIGHT GLINTS (Mordor, 2026-10-10, from his photo of glittering moonlit snow): by night, single pale blue-white pixels
// sparkle on the open snow. The ground is cut into GLINT_CELL x GLINT_CELL world-px cells; GLINT_CHANCE of them hold one
// glint SITE at a hashed spot inside (fewer in the art's baked shadows). A site flashes when two slow waves meet: one in
// TIME (its own speed and phase) and one in the VIEWPOINT -- the camera's world position along the site's own direction,
// so walking makes glints flash on and off like real snow does as the angle to the moon changes. The brightest flashes
// light a faint 1-px cross around them (GLINT_CROSS) -- worked out from the same site, so the cross costs nothing extra.
// COST: one site per cell is what keeps this cheap (~100 instructions, branched away by day and on most cells): the first
// version tested every pixel AND its four neighbours as possible sites -- 786 instruction slots on most of the screen.
// Fixed to the ground; no glints by day (the lit snow is already white there). Hash: CloudShadow.hlsl's (sine-free; its
// lattice repeats every 289 cells = 1734 world px, wider than the screen).
sampler2D frame : register(s0);
sampler2D mask : register(s1);
sampler2D ground : register(s2);
sampler2D lut : register(s3);
sampler2D world : register(s4);
float4 toLayer : register(c0);
float4 lay : register(c1);
float4 litCol : register(c2);
float4 shadeCol : register(c3);
float4 tx : register(c4);
float4 seeBox : register(c5);
float4 glintK : register(c6);

static const float  GLINT_CELL    = 6.0;	// world px per cell (one possible site in each)
static const float  GLINT_CHANCE  = 0.07;	// share of the cells that hold a site (0.07 / 36 = ~0.19% of the snow's pixels; Mordor)
static const float  GLINT_SHADOW  = 0.35;	// ... x this in the art's baked shadows
static const float3 GLINT_RGB     = float3(0.86, 0.93, 1.00);	// a glint at full flash
static const float  GLINT_SHARP   = 9.0;	// higher = shorter flashes, longer dark between them (Mordor)
static const float  GLINT_SPEED   = 0.9;	// time wave, radians per second (each site 0.6 .. 1.4 x this)
static const float  GLINT_VIEW    = 0.16;	// viewpoint wave, radians per world px the camera moves (~40 px per flash)
static const float  GLINT_CROSS   = 0.35;	// the arms around a flash brighter than GLINT_CROSS_AT
static const float  GLINT_CROSS_AT = 0.7;

// (th4) HASH WITHOUT SINE -- the same function as CloudShadow / Mist / Frost / TreeSway.hlsl (see CloudShadow.hlsl)
float hash(float2 p)
{
	p = p - 289.0 * floor((p + 0.5) / 289.0);
	float3 p3 = frac(float3(p.xyx) * 0.1031);
	p3 += dot(p3, p3.yzx + 33.33);
	return frac((p3.x + p3.y) * p3.z);
}
// the glint 0..1 at world pixel ip (whole numbers): its cell's site, if any, at the centre or as a cross arm
float glint(float2 ip, float chance)
{
	float2 cell = floor(ip / GLINT_CELL);
	float h0 = hash(cell);
	[branch] if (h0 >= chance) return 0.0;		// most cells: no site
	float h1 = hash(cell + 37.0), h2 = hash(cell + 71.0);
	float2 site = cell * GLINT_CELL + 1.0 + floor(float2(h1, h2) * (GLINT_CELL - 2.0));	// inside, arms stay in the cell
	float2 o = abs(ip - site);
	float ring = o.x + o.y;
	[branch] if (ring > 1.5) return 0.0;			// not the site nor an arm
	float a = h2 * 6.2832, ph = h1 * 6.2832;
	float view = sin(dot(glintK.xy, float2(cos(a), sin(a))) * GLINT_VIEW + ph);
	float time = sin(glintK.z * GLINT_SPEED * (0.6 + 0.8 * h0 / chance) + ph * 1.7);
	float f = pow(saturate((view * 0.5 + 0.5) * (time * 0.5 + 0.5)), GLINT_SHARP) * (1.0 + 2.0 * GLINT_SHARP) * 0.25;
	return ring < 0.5 ? saturate(f) : saturate((f - GLINT_CROSS_AT) / (1.0 - GLINT_CROSS_AT)) * GLINT_CROSS;
}

float4 main_snowground(float2 uv : TEXCOORD0) : COLOR
{
	float3 c = tex2D(frame, uv).rgb;
	float worldW = saturate(tex2D(mask, uv).r * (255.0 / 80.0));
	if (all(uv >= seeBox.xy) && all(uv < seeBox.zw))
		worldW = tex2Dlod(world, float4(uv, 0, 0)).r;
	if (worldW <= 0.0)
		return float4(c, 1.0);
	float2 w = uv * toLayer.xy + toLayer.zw;
	float v = tex2Dlod(ground, float4((floor(w) + 0.5) * lay.xy, 0, 0)).r;
	if (v <= 0.0)
		return float4(c, 1.0);			// not open ground
	float u = (floor(v * 255.0 + 0.5) + 0.5) / 256.0;
	float artLum = tex2Dlod(lut, float4(u, 0.25, 0, 0)).r;
	float palLum = tex2Dlod(lut, float4(u, 0.75, 0, 0)).r;
	float lum = dot(c, float3(0.299, 0.587, 0.114));
	float a = max(artLum, 0.03);
	float light = min(lum / a, min(lay.w, palLum / a * tx.w));	// the light the game gave this pixel (a flake: its art's)
	float shade = smoothstep(shadeCol.w, litCol.w, artLum);
	float grain = 1.0 - tx.x + tx.x * saturate((artLum - tx.y) * tx.z);
	float3 snow = lerp(shadeCol.rgb, litCol.rgb, shade) * grain * light;
	[branch] if (glintK.w > 0.0) {				// MOONLIGHT GLINTS (above), by night
		float2 ip = floor(w - glintK.xy);		// the world pixel
		float g = glint(ip, GLINT_CHANCE * lerp(GLINT_SHADOW, 1.0, shade));
		snow = lerp(snow, GLINT_RGB, g * glintK.w);
	}
	return float4(lerp(c, saturate(snow), lay.z * worldW), 1.0);
}
