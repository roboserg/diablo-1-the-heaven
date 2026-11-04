uniform sampler2D image;
uniform sampler1D palette;

// A8L8 texture: .r = palette index (L byte / 255), .a = light level (A byte / 255)
// A byte is full 0-255 range: 0=full bright, 255=pitch black.
// GPU normalizes: samp.a = A_byte/255 → brightness = saturate(1.0 - samp.a).
// 256 distinct brightness levels eliminate color banding and sawtooth artifacts.
static const float LIGHT_SCALE = 1.0; // A byte range is 0-255

float4 main_palette(in float2 coord:TEXCOORD) : COLOR
{
	#if defined(MORE_COLOR)
	return tex1D(palette, tex2D(image, coord).r * (65534./512) + (0.5/512));
	#else
	float4 samp = tex2D(image, coord);
	float4 color = tex1D(palette, samp.r * (255./256) + (0.5/256));
	color.rgb *= saturate(1.0 - samp.a * LIGHT_SCALE);
	return color;
	#endif
}