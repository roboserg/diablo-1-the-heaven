sampler tx : register(s0);
sampler pl : register(s1);

struct PSInput{
	float2 sp : VPOS;
	float2 cr : TEXCOORD0;
};

float4 main_bloom(PSInput input) : COLOR
{
	float w = 1.0 / (1.0 / input.cr.x * input.sp.x);
	float h = 1.0 / (1.0 / input.cr.y * input.sp.y);
	float4 px = tex1D(pl, tex2D(tx, input.cr).r);

	/* {  }{  }[d2]{  }{  }
	*  {  }[x1]{  }[x3]{  }
	*  [d4]{  }[px]{  }[d5]
	*  {  }[x6]{  }[x8]{  }
	*  {  }{  }[d7]{  }{  }
	*/
	
	float4 x1 = tex1D(pl, tex2D(tx, input.cr + float2(-w,-h)).r);
	float4 x3 = tex1D(pl, tex2D(tx, input.cr + float2(w, h)).r);
	float4 x6 = tex1D(pl, tex2D(tx, input.cr + float2(w,-h)).r);
	float4 x8 = tex1D(pl, tex2D(tx, input.cr + float2(-w, h)).r);

	float4 d2 = tex1D(pl, tex2D(tx, input.cr + float2(0, -(h/2))).r);
	float4 d4 = tex1D(pl, tex2D(tx, input.cr + float2(-(w/2), 0)).r);
	float4 d5 = tex1D(pl, tex2D(tx, input.cr + float2(w/2, 0)).r);
	float4 d7 = tex1D(pl, tex2D(tx, input.cr + float2(0, h/2)).r);

	float4 m = (x1 + d2 + x3 + d4 + d5 + x6 + d7 + x8) / 8;
	m *= m;
	return clamp(px + m,0.0,1.0);
	

}