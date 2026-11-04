// SnowFlakes.hlsl -- (new by th4) SNOWFALL ON THE GPU, the pixel half (the vertex half and the inputs: SnowFlakesVS.hlsl;
// driver dx_utility.cpp Snowfall_Present, numbers Snowfall.cpp SNOWFALL ON THE GPU). A RICE GRAIN: an ellipse with the
// grain's half length along its flight and half width across, its edge soft over shape.z su -- across that is about one
// window px, along it the tips fade out longer, like motion blur. Written as its colour x alpha x cover x world and blended
// SCREEN (dx_utility.cpp Snowfall_Present: frame + grain x ( 1 - frame )): it only ever brightens.
// s0 = world/UI mask from HeatHaze_BuildMask, sampled LINEAR: 0 = UI, >= 80/255 = world (as Rain.hlsl).
// s1 = the FULL-RES WORLD MAP (POINT; 1 = world, 0 = UI), used inside the c0 box only
// c0 = the see-through dialog box in screen uv (x0, y0, x1, y1; all 2 = none): the town dialog / NPC speech box lets the
//      world show through its 50 % dither, but the half-res mask calls the whole box UI (every 2 x 2 holds a dot), so the
//      flakes stopped at the box (Mordor). Inside it the world pixels between the dots come from s1 -- as SnowGround.hlsl c5.
sampler2D mask : register(s0);
sampler2D world : register(s1);
float4 seeBox : register(c0);

float4 main_snowflakes( float4 col : COLOR0, float2 local : TEXCOORD0, float2 uv : TEXCOORD1, float3 shape : TEXCOORD2 ) : COLOR
{
	const float e = length( local / shape.xy );	// 1 = the ellipse's rim
	const float cover = saturate( ( 1.0 - e ) * shape.y / shape.z + 0.5 );
	float w = saturate( tex2D( mask, uv ).r * ( 255.0 / 80.0 ) );
	if( all( uv >= seeBox.xy ) && all( uv < seeBox.zw ) )
		w = tex2Dlod( world, float4( uv, 0, 0 ) ).r;
	return float4( col.rgb * ( col.a * cover * w ), 1.0 );
}
