uniform sampler2D image;
uniform sampler1D palette;
//sampler TextureSampler = sampler_state {
//	texture = <D3dTexture>;
//    MipFilter = LINEAR;
//    MinFilter = LINEAR;
//    MagFilter = LINEAR;
//};

float4 main_palette(in float2 coord:TEXCOORD) : COLOR
{
	#if defined(MORE_COLOR)
	return tex1D(palette, tex2D(image, coord).r * (65534./512) + (0.5/512));
	#else
	return tex1D(palette, tex2D(image, coord).r * (255./256) + (0.5/256));
	#endif
}