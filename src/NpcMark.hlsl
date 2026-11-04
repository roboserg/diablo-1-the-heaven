// NpcMark.hlsl -- (new by th4) the NPC MARK: the "!" over a towner (or a Dark Campaign NPC) who has something
// to say, in the look of Diablo II's -- a see-through, softly glowing pale-yellow speech bubble with its tail at
// the lower left, a brighter band along its top rim, and a dark "!" in it. Drawn by code here; no art file.
// Driver: dx_utility.cpp NpcMark_Present (NPC MARKS); NpcMark.cpp has the whole story (who gets a mark, where,
// what covers it). One small quad per mark, ALPHA-BLENDED over the finished frame (premultiplied: ONE,
// INVSRCALPHA) -- nothing is copied or read back.
// THE SHAPE lives here alone, in 1x game px from the mark's centre (x right, y down): bubble half-size BUBBLE,
// tail TAIL_*, "!" BAR_* / DOT_*, glow GLOW_*. NpcMark.cpp NPC_MARK_LEFT / RIGHT / UP / DOWN say how far the
// whole thing (glow included) can reach -- they size the quad and the cover box, so change them with the shape.
// t0 = the position in the mark: 1x game px from its centre -- the quad is mapped so, whatever the window size,
//      Alt+Z or the wheel zoom
// s0 = the COVER (linear, clamp): 1 where the UI was drawn over the mark (panels, text, the cursor), 0 where the
//      world shows; NpcMark.cpp NpcMark_Cover works it out per mark from the 8-bit frame, one cell per mark
// c0 = x one window px in mark units (edge softness), y opacity (the palette fade), z 1 = the red screen
//      (pause, death, Terminator vision), w unused
// c1 = rgb the bubble under the red screen, c2 = rgb the "!" under the red screen (NpcMark.cpp
//      NpcMark_OnRedScreen: the colours the 8-bit red screen turns them into)
// c3 = where this mark's cover cell is: its uv = t0 * c3.xy + c3.zw
sampler2D cover : register(s0);
float4 k : register(c0);
float4 redLens : register(c1);
float4 redInk : register(c2);
float4 coverMap : register(c3);

// measured from a Diablo II screenshot (the bubble over grass, solved for its own colour and opacity)
static const float2 BUBBLE = float2(14.0, 9.5);			// half-size of the ellipse
static const float2 TAIL_A = float2(-7.5, 6.5);			// the tail: a triangle from the lower left of the
static const float2 TAIL_B = float2(-1.0, 8.5);			// ellipse (A, B inside it) to its tip
static const float2 TAIL_TIP = float2(-5.5, 14.5);
static const float  JOIN = 2.0;							// how round the corners where the tail joins, px
static const float  EDGE = 1.3;							// the soft rim, px
static const float  GLOW_A = 0.36, GLOW_W = 2.0;		// the haze around it: opacity at the rim, fall-off px
static const float3 LENS  = float3(0.98, 0.92, 0.62);	// pale cream yellow
static const float  LENS_A = 0.62;						// see-through
static const float3 GLINT = float3(1.00, 0.99, 0.88);	// the bright band along the top rim
static const float3 INK   = float3(0.13, 0.09, 0.01);	// the "!": dark brown, nearly opaque
static const float  INK_A  = 0.94;
static const float  BAR_TOP = -4.7, BAR_FOOT = -0.3;		// the bar: a segment of these ends ...
static const float  BAR_R_TOP = 1.35, BAR_R_FOOT = 0.95;	// ... and this thickness, narrowing down
static const float2 DOT = float2(0.0, 4.0);
static const float  DOT_R = 1.25;
static const float3 LUM = float3(0.299, 0.587, 0.114);

// signed distance to a triangle (negative inside) -- the usual exact formula
float sdTri(float2 p, float2 p0, float2 p1, float2 p2)
{
	float2 e0 = p1 - p0, e1 = p2 - p1, e2 = p0 - p2;
	float2 v0 = p - p0, v1 = p - p1, v2 = p - p2;
	float2 q0 = v0 - e0 * saturate(dot(v0, e0) / dot(e0, e0));
	float2 q1 = v1 - e1 * saturate(dot(v1, e1) / dot(e1, e1));
	float2 q2 = v2 - e2 * saturate(dot(v2, e2) / dot(e2, e2));
	float s = sign(e0.x * e2.y - e0.y * e2.x);
	float2 d = min(min(float2(dot(q0, q0), s * (v0.x * e0.y - v0.y * e0.x)),
	                   float2(dot(q1, q1), s * (v1.x * e1.y - v1.y * e1.x))),
	                   float2(dot(q2, q2), s * (v2.x * e2.y - v2.y * e2.x)));
	return -sqrt(d.x) * sign(d.y);
}

float4 main_npcmark(float2 p : TEXCOORD0) : COLOR
{
	float aa = max(k.x, 0.3);
	// ---- the bubble: how far inside its edge, px (ellipse: r - 1 over the length of its gradient), with the tail
	// joined on by a smooth max so the corners round off like a drawn speech bubble ----
	float2 q = p / BUBBLE;
	float r = max(length(q), 1e-4);
	float dE = (1.0 - r) * r / max(length(q / BUBBLE), 1e-4);
	float dT = -sdTri(p, TAIL_A, TAIL_B, TAIL_TIP);
	float h = max(JOIN - abs(dE - dT), 0.0) / JOIN;
	float d = max(dE, dT) + h * h * JOIN * 0.25;
	float body = smoothstep(0.0, EDGE + aa, d);
	float3 col = LENS * (1.0 + 0.10 * (-0.45 * q.x - 0.9 * q.y));		// a little brighter to the upper left
	float a = LENS_A * (1.0 - 0.25 * smoothstep(0.2, 1.4, q.y));			// a little thinner toward the bottom
	float glint = smoothstep(0.66, 0.84, r) * (1.0 - smoothstep(0.93, 1.02, r)) * smoothstep(0.35, 0.85, -q.y / r);
	col = lerp(col, GLINT, glint * 0.85);
	a = lerp(a, 0.88, glint);
	a = a * body + GLOW_A * exp(min(d, 0.0) / GLOW_W) * (1.0 - body);		// inside, or the haze outside
	// ---- the "!": a bar that narrows downward and a dot, as signed distances (px) ----
	float t = saturate((p.y - BAR_TOP) / (BAR_FOOT - BAR_TOP));
	float dBar = length(p - float2(0.0, clamp(p.y, BAR_TOP, BAR_FOOT))) - lerp(BAR_R_TOP, BAR_R_FOOT, t);
	float dDot = length(p - DOT) - DOT_R;
	float inkA = INK_A * saturate(0.5 - min(dBar, dDot) / max(aa, 0.55));
	// ---- the red screen: the colours the 8-bit XLAT gives, keeping the bubble's own shading ----
	float3 inkC = INK;
	if (k.z > 0.5)
	{
		col = redLens.rgb * (dot(col, LUM) / dot(LENS, LUM));
		inkC = redInk.rgb;
	}
	// ---- the "!" over the bubble, premultiplied; the UI on top (cover), the palette fade ----
	float outA = inkA + a * (1.0 - inkA);
	float3 outC = inkC * inkA + col * a * (1.0 - inkA);
	float vis = k.y * (1.0 - tex2D(cover, p * coverMap.xy + coverMap.zw).r);
	return float4(outC * vis, outA * vis);
}
