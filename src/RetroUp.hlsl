// RetroUp.hlsl -- (new by th4) RETRO MODES, pass 2 of 2: enlarges the low-resolution retro image
// (Retro.hlsl, pass 1) to the window. Driver: dx_utility.cpp Retro_Present.
// "Sharp bilinear": every retro pixel stays a crisp block and only the one window pixel on each
// block seam is blended. Plain POINT scaling at a non-integer ratio (e.g. 1056 -> 1920, x1.82)
// makes blocks alternately 1 and 2 window pixels wide, which turns dither checkerboards into
// uneven stripes; plain LINEAR would blur the pixel art. Same idea as SharpStretch, as a shader.
//
// s0 = low-res retro image (LINEAR, clamp)
// c0 = xy low-res texture size, zw low-res extent actually covering the game frame
//      (game size / BLOCK -- the last row / column of the texture may be partial)
// c1 = xy window px per retro pixel
sampler2D low : register(s0);
float4 u0 : register(c0);
float4 u1 : register(c1);

float4 main_retroup(float2 uv : TEXCOORD0) : COLOR
{
	float2 p = uv * u0.zw;	// in retro pixels
	float2 fl = floor(p);
	float2 d = p - fl - 0.5;	// -0.5 .. 0.5 from the block centre
	float2 region = max(0.5 - 0.5 / u1.xy, 0.0);	// flat part of the block
	float2 f = (d - clamp(d, -region, region)) * u1.xy + 0.5;
	return float4(tex2D(low, (fl + f) / u0.xy).rgb, 1);
}
