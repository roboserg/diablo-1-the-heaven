// SnowFlakesVS.hlsl -- (new by th4) SNOWFALL ON THE GPU, the vertex half (the pixel half: SnowFlakes.hlsl).
// Driver: dx_utility.cpp Snowfall_Present (PostFxPresent, after the breath); the numbers: Snowfall.cpp SNOWFALL ON THE GPU.
// One INSTANCED quad per flake: stream 0 = the quad's 4 corners (POSITION, -1 / +1), stream 1 = the flake (TEXCOORD0..1,
// Snowfall_GpuFlakes, written once per field size). The flake moves exactly as the 8-bit flakes do (Snowfall_Render):
// start + its layer's travel x its speed factor (80..120 %) + its churn, wrapped into the field, minus the margin -- but
// continuously, in SNOW UNITS (su = px of a 1080-high window; c0.z = window px per su). The quad lies along the flake's own
// flight right now (its layer's wind and fall x its speed factor + its churn's motion), as long as its RICE GRAIN
// (max( the layer's minimum, speed x exposure )), as wide as the grain, plus the soft edge.
// c0 = x window width px, y height px, z px per su, w field width su
// c1 = x field height su, y margin su, z snow clock ms (wrapped at 256000, where every churn rate turns whole),
//      w the storm 0..1 (Snowfall_Storm): a flake's a.w = layer + 0.98 x its storm threshold (0 = always there); an EXTRA
//      flake fades in over the last 0.1 below its threshold and is collapsed off screen while it is gone
// c2 + 4 L (L = layer 0 far, 1 mid, 2 near):
//   [0] x, y travel su (wrapped at 100 x the field, where every speed factor turns whole), z, w churn x, y su
//   [1] x wind, y fall su / s (this frame, at 100 %), z exposure s, w minimum length su
//   [2] colour rgb, alpha
//   [3] x half width su, y soft edge su, z churn rate scale (in tenths: Snowfall.cpp kGpuGrains)
float4 g0 : register(c0);
float4 g1 : register(c1);
float4 lay[12] : register(c2);

struct VsIn {
	float2 corner : POSITION;
	float4 a : TEXCOORD0;	// x0, y0 su, speed 0..40, layer 0..2 + 0.98 x storm threshold
	float4 b : TEXCOORD1;	// churn phase x, y 0..255, churn rate x, y 0..15
};
struct VsOut {
	float4 pos : POSITION;
	float4 col : COLOR0;
	float2 local : TEXCOORD0;	// su from the grain's centre: x along its flight, y across
	float2 uv : TEXCOORD1;		// window uv, for the world/UI mask
	float3 shape : TEXCOORD2;	// x half length, y half width, z soft edge (su)
};

VsOut main_snowflakesvs( VsIn i )
{
	VsOut o;
	const float lw = floor( i.a.w + 0.001 );
	const int L = (int)lw * 4;
	const float thr = ( i.a.w - lw ) / 0.98;
	const float vis = thr > 0.001 ? saturate( ( g1.w - thr ) / 0.1 + 1.0 ) : 1.0;
	const float4 T = lay[ L ], V = lay[ L + 1 ], C = lay[ L + 2 ], S = lay[ L + 3 ];
	const float f = 0.8 + 0.01 * i.a.z;
	const float2 field = float2( g0.w, g1.x );
	const float2 rate = ( 300.0 + 40.0 * i.b.zw ) / 1000.0 * S.z;	// the churn's phase per ms, in 1/256 turns (Snowfall_Render ratePhase, x the layer's scale)
	const float2 ang = 6.2831853 * frac( ( g1.z * rate + i.b.xy ) / 256.0 );
	float2 p = i.a.xy + T.xy * f + T.zw * sin( ang );
	p = frac( p / field ) * field - g1.y;
	const float2 v = V.xy * f + T.zw * cos( ang ) * ( 6.2831853 * 1000.0 / 256.0 ) * rate;	// su / s: the layer's flight + the churn's own
	const float sp = max( length( v ), 0.001 );
	const float2 d = v / sp, n = float2( -d.y, d.x );
	const float halfLen = 0.5 * max( V.w, sp * V.z ), hw = S.x, soft = S.y;
	// the ellipse fades out at 1 + 0.5 soft / hw of its radii (SnowFlakes.hlsl): the quad reaches just past that
	const float2 local = i.corner * float2( halfLen * ( 1.0 + 0.5 * soft / hw ) + 0.5, hw + 0.5 * soft + 0.5 );
	const float2 px = ( p + d * local.x + n * local.y ) * g0.z;	// window px (pixel i covers i .. i + 1)
	o.pos = vis > 0.0 ? float4( 2.0 * px.x / g0.x - 1.0, 1.0 - 2.0 * px.y / g0.y, 0.0, 1.0 ) : float4( 2.0, 2.0, 0.0, 1.0 );	// gone: off screen
	o.col = float4( C.rgb, C.a * vis );
	o.local = local;
	o.uv = px / g0.xy;
	o.shape = float3( halfLen, hw, soft );
	return o;
}
