// Vhs.hlsl -- (new by th4) RetroMode 19 "VHS tape": the whole screen as a worn video tape on an old VCR.
// Driver: dx_utility.cpp Vhs_Present, called by Retro_Present for the VHS row of RetroModes[] (kind 8). Window
// size, one pass: the frame copy in s0 (LINEAR) is redrawn. Fully procedural, nothing is kept between frames.
// WHAT A TAPE DOES TO A PICTURE, and what this shader does about each part:
//   - LUMA (brightness) is recorded at ~240 lines per picture height: soft sideways, and the VCR's sharpening
//     circuit rings around edges (a light fringe beside dark edges)  -> 7 taps across + overshoot (VHS_RING).
//   - CHROMA (colour) only ~30 lines: colour smears sideways and lags to the right  -> 9 wide taps on the I/Q
//     colour channels, centred a little to the LEFT of the pixel, so the colour lands to the right of its
//     object (VHS_CHROMA_LAG). Kept at 48 lines, not 30: at 30 the colour of item names and small text is gone.
//   - Tape tone: blacks lifted, whites a little compressed, colour slightly turned and warmed.
//   - TIMEBASE: every line sits a hair off sideways and drifts; the top lines bend ("flagging"); the last lines
//     tear off to one side ("head switching")  -> a sideways offset per video line; the picture's black edges
//     wobble with it.
//   - TRACKING: now and then a band of torn, noisy lines rolls through the picture (c2, timed on the CPU).
//   - Snow (fine luma noise), coloured chroma blotches, and rare white DROPOUT streaks where the tape shed oxide.
//   - PAUSE (c1.z): the picture hops a fraction of a line up and down and two noise bars sit across it.
// Everything is measured in VIDEO LINES (480 per picture height) and picture heights, so it looks the same at
// every resolution and window size. CRT / film grain (PostFx.hlsl) run after this pass and lay on top.
// s0 = frame copy (linear, clamp)
// c0 = xy window size, zw 1 / window size
// c1 = x clock in seconds (REAL time: the tape runs on while the game is paused), y frame number 0..996,
//      z 1 = paused, w aspect (width / height)
// c2 = tracking band: x centre (0..1 of the height; far outside = none), y strength 0..1, z half height
sampler2D frame : register(s0);
float4 g : register(c0);
float4 k : register(c1);
float4 tb : register(c2);

static const float LINES = 480.0;			// video lines per picture height
static const float LUMA_LINES = 240.0;		// luma resolution, lines per picture height
static const float CHROMA_LINES = 48.0;		// chroma resolution (a real tape has ~30; 48 keeps item colours readable)
static const float VHS_RING = 0.55;			// the VCR's edge sharpening (overshoot)
static const float VHS_CHROMA_LAG = 0.35;	// colour lag to the right, in chroma samples
static const float VHS_SAT = 1.12;			// colour strength after the smear (the smear itself thins small colour areas)
static const float3 kY = float3(0.299, 0.587, 0.114);
static const float3 kI = float3(0.596, -0.274, -0.322);
static const float3 kQ = float3(0.211, -0.523, 0.312);

// per-cell noise, SINE-FREE on purpose: the usual frac(sin(dot(p, ...)) * 43758) hash needs sin() of numbers in the
// millions here (snow cells x frame), and ps_3_0 reduces sin's argument in float32 -- out there only ~16 phases are
// left and the snow turns into stripes. This frac / dot shape (Dave Hoskins' "hash without sine" idea) stays
// good while the inputs stay below ~20000; every call below keeps them there.
float hash(float2 p)
{
	float3 p3 = frac(p.xyx * 0.1031);
	p3 += dot(p3, p3.yzx + 33.33);
	return frac((p3.x + p3.y) * p3.z);
}

float luma(float2 p) { return dot(tex2D(frame, p).rgb, kY); }

float4 main_vhs(float2 uv : TEXCOORD0) : COLOR
{
	float t = k.x, fr = k.y, paused = k.z, aspect = k.w;
	if (paused > 0.5)
		uv.y += (fmod(fr, 2.0) - 0.5) * (1.2 / LINES);			// a paused VCR: the frame hops up and down
	float vl = floor(uv.y * LINES);						// this video line ("line" is an HLSL keyword)

	// ---- timebase: how far this line sits sideways, in picture heights ----
	float off = (hash(float2(vl, fr)) - 0.5) * 0.0012;			// every line a hair off, new each frame
	off += sin(vl * 0.043 + t * 1.7) * 0.0006;					// a slow wave down the picture
	float top = saturate(1.0 - uv.y / 0.07);						// flagging: the top lines bend
	off += top * top * 0.010 * sin(t * 0.9 + vl * 0.02);
	float bottom = saturate((uv.y - 0.975) / 0.025);				// head switching: the last lines tear off
	off += bottom * (0.035 + (hash(float2(vl, fr + 7.0)) - 0.5) * 0.03);
	// tracking band from the CPU; while paused, the two fixed noise bars of a paused tape
	float band = tb.y * saturate(1.0 - abs(uv.y - tb.x) / tb.z);
	if (paused > 0.5)
	{
		float b1 = saturate(1.0 - abs(uv.y - 0.27 - (hash(float2(fr, 3.0)) - 0.5) * 0.004) / 0.022);
		float b2 = saturate(1.0 - abs(uv.y - 0.79 - (hash(float2(fr, 5.0)) - 0.5) * 0.004) / 0.016);
		band = max(band, max(b1, b2));
	}
	band = band * band * (3.0 - 2.0 * band);
	off += band * (hash(float2(vl, fr + 13.0)) - 0.5) * 0.06;
	float2 p = float2(uv.x + off / aspect, uv.y);					// picture heights -> uv across

	// ---- luma: soft across, with the VCR's ringing ----
	float ls = 0.5 / LUMA_LINES / aspect;							// half a luma sample, in uv
	float y0 = luma(p);
	float y1 = luma(p + float2(ls, 0)) + luma(p - float2(ls, 0));
	float y2 = luma(p + float2(2.0 * ls, 0)) + luma(p - float2(2.0 * ls, 0));
	float y3 = luma(p + float2(3.0 * ls, 0)) + luma(p - float2(3.0 * ls, 0));
	float yN = y0 * 0.4 + y1 * 0.3;									// narrow
	float yW = (y0 + y1 + y2 + y3) / 7.0;							// wide
	float Y = yN + VHS_RING * (yN - yW);

	// ---- chroma: wide smear, lagging to the right ----
	float cs = 1.0 / CHROMA_LINES / aspect / 3.0;					// a third of a chroma sample, in uv
	float2 cp = p - float2(cs * 3.0 * VHS_CHROMA_LAG, 0);			// sample to the left = the colour lags right
	float2 iq = 0;
	[unroll] for (int i = -4; i <= 4; ++i)
	{
		float3 c = tex2D(frame, cp + float2(cs * i, 0)).rgb;
		iq += float2(dot(c, kI), dot(c, kQ)) * (1.0 - abs(i) / 5.0);
	}
	iq *= 0.2;														// the weights sum to 5

	// ---- tape tone ----
	Y = pow(saturate(0.035 + Y * 0.93), 1.06);						// blacks lifted, whites softer
	const float hc = 0.9976, hs = -0.0699;							// colour turned ~4 degrees
	iq = float2(iq.x * hc - iq.y * hs, iq.x * hs + iq.y * hc) * VHS_SAT + float2(0.006, -0.002);

	// ---- noise ----
	float sx = floor(uv.x * aspect * LINES);						// snow: cells one line wide and high
	Y += (hash(float2(sx + fr * 13.0, vl)) - 0.5) * (0.035 + band * 0.25 + bottom * 0.2);
	float cx = floor(uv.x * aspect * 60.0), cy = floor(vl * 0.5);	// chroma blotches, stretched sideways
	iq += (float2(hash(float2(cx, cy + fr * 3.0)), hash(float2(cx + 11.0, cy + fr * 3.0))) - 0.5) * 0.035;
	iq *= 1.0 - band * 0.6;											// torn lines lose their colour
	// white noise in the band: broken dashes on some lines, not whole lines (a whole white line looks digital)
	float dash = floor(uv.x * aspect * 36.0 + hash(float2(vl, fr + 300.0)) * 36.0);
	float bandLine = step(0.7, hash(float2(vl, fr + 200.0))) * step(0.45, hash(float2(dash, vl + fr * 2.0)));
	Y = lerp(Y, 0.9, band * bandLine * 0.7);
	Y = lerp(Y, Y * 0.7 + 0.15, bottom * 0.5);
	// dropouts: a short white streak on one line, now and then (more of them in a tracking band)
	if (hash(float2(vl, fr + 101.0)) > 0.9992 - band * 0.02)
	{
		float x0 = hash(float2(vl + 17.0, fr)), len = 0.02 + 0.10 * hash(float2(vl + 29.0, fr));
		float u2 = (uv.x - x0) / len;
		if (u2 > 0.0 && u2 < 1.0)
			Y = lerp(Y, 1.0, 0.75 * sin(u2 * 3.14159));
	}

	// ---- the picture's black edges (they wobble with the timebase) and a soft vignette ----
	float edge = smoothstep(0.0, 0.006, p.x) * smoothstep(1.0, 0.994, p.x);
	float2 d = (uv - 0.5) * float2(1.0, 1.15);
	float v = (1.0 - dot(d, d) * 0.35) * edge;
	Y *= v; iq *= v;

	float3 rgb = float3(Y + 0.956 * iq.x + 0.621 * iq.y,
	                    Y - 0.272 * iq.x - 0.647 * iq.y,
	                    Y - 1.106 * iq.x + 1.703 * iq.y);
	return float4(saturate(rgb), 1.0);
}
