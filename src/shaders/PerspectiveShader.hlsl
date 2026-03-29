sampler tx : register(s0);
sampler pl : register(s1);

#define w (1.0/640)
#define h (1.0/320)
#define w2 (1.0/320)
#define h2 (1.0/160)

float4 main_perspective(float2 cr : TEXCOORD0) : COLOR
{


	/*  Y   ________
	*   v  /|      |\
	*     / |      | \
	*    /__|______|__\
	*   X >
	*/
	float y = cr.y * 0.2;
	float x = cr.x - (cr.x * y) + y/2;
	return tex1D(pl, tex2D(tx, float2(x,cr.y)).r);
}