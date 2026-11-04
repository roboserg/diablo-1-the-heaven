// (th4) DYNAMIC ZOOM composite -- dx_utility.cpp "DYNAMIC ZOOM" (Zoom_Composite), DynamicZoom.cpp (the design).
// One pass into the scene's target (game resolution, or the back buffer when the scene quad draws there):
//   UI pixels    (mask = 1) -> the finished frame, unscaled (finalTex, the scene shader's output for world+UI);
//   world pixels (mask = 0) -> the world-only frame (worldTex, the scene shader's output for the world snapshot)
//                              scaled by z about the hero: q = c + (p - c) / z.
// The world is sampled SHARP-BILINEAR: every source texel stays a flat square and only a seam about one target
// pixel wide is blended between neighbours. Pixel art stays crisp at any factor, and there are none of the
// uneven rows / columns (and the crawl while walking) that nearest-neighbour gives at a fractional scale.
// Built by FxCompile like the other shaders (ps_3_0, entry main_zoom -> ZoomComposite.h, g_ps30_main_zoom).
sampler2D finalTex : register(s0);	// POINT
sampler2D worldTex : register(s1);	// LINEAR (the sharp-bilinear trick needs the hardware blend)
sampler2D maskTex  : register(s2);	// POINT, L8: 1 = UI
float4 zc : register(c0);			// x = 1/z, y = centre x, z = centre y (game px), w = z
float4 rs : register(c1);			// x = game width, y = game height, z = 1/width, w = 1/height

float4 main_zoom( float2 uv : TEXCOORD0 ) : COLOR
{
	float4 fin = tex2D( finalTex, uv );
	float  ui  = tex2D( maskTex, uv ).r;
	float2 p = uv * rs.xy;							// game px of this pixel's centre
	float2 q = zc.yz + ( p - zc.yz ) * zc.x;		// the same point in the 1x world frame
	float2 t = q - 0.5;								// texel-centre space
	float2 i = floor( t );
	float2 f = saturate( ( t - i - 0.5 ) * zc.w + 0.5 );	// sharpen the blend to ~one target pixel
	float4 w = tex2D( worldTex, ( i + 0.5 + f ) * rs.zw );
	return ui > 0.5 ? fin : w;
}
