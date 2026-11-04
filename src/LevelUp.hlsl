// LevelUp.hlsl -- (new by th4) LEVEL-UP GLOW (config LevelUpGlow on/off): a golden ring bursts out
// over the ground from where the hero stood when the level came, with a flash at his feet, a
// pillar of light above him and specks rising in it. Driver: dx_utility.cpp LevelUp_Present
// (ATMOSPHERE block); started by LevelUpFx_Start(), called from Player.cpp NextPlayerLevel for the
// local player. About LEVELUP_SEC long; the ring lies on the 2:1 floor (vertical distance doubled).
// Window size, one pass, added over WORLD pixels only (soft mask as Mist.hlsl).
//
// s0 = frame copy (point, clamp), s1 = world/UI mask (linear)
// c0 = xy game resolution, z zoom (1 or 2), w progress 0..1
// c1 = xy centre (game px, the hero's ground point), zw unused
sampler2D frame : register(s0);
sampler2D mask : register(s1);
float4 g : register(c0);
float4 ctr : register(c1);

float sq(float x) { return x * x; }
float hash(float2 p) { return frac(sin(dot(p, float2(127.1, 311.7))) * 43758.5453); }

float4 main_levelup(float2 uv : TEXCOORD0) : COLOR
{
	float3 c = tex2D(frame, uv).rgb;
	float worldW = saturate(tex2D(mask, uv).r * (255.0 / 80.0));
	if (worldW <= 0.0) return float4(c, 1.0);
	float t = g.w;
	float2 p = (uv * g.xy - ctr.xy) / g.z;		// world px from the hero's feet
	float e = length(float2(p.x, p.y * 2.0));	// distance on the floor
	float ease = 1.0 - (1.0 - t) * (1.0 - t) * (1.0 - t);
	float R = ease * 170.0;
	float ring = exp(-sq((e - R) / (6.0 + 6.0 * t))) * pow(saturate(1.0 - t), 1.5);
	float flash = exp(-sq(e / 38.0)) * saturate(1.0 - t * 3.0);
	float up = -p.y;							// height above the feet
	float pillar = exp(-sq(p.x / 16.0)) * saturate(1.0 - up / 260.0) * smoothstep(-12.0, 6.0, up) * (1.0 - t) * (1.0 - t);
	// specks rising in the pillar: one per 8x10 px cell, stateless
	float2 q = float2(p.x, up - t * 150.0);
	float2 id = floor(q / float2(8.0, 10.0));
	float h = hash(id);
	float2 sp = (id + float2(hash(id + 1.7), hash(id + 4.1))) * float2(8.0, 10.0);
	float speck = (h > 0.82 ? 1.0 : 0.0) * exp(-dot(q - sp, q - sp) / 1.6) * exp(-sq(p.x / 30.0))
	            * saturate(1.0 - up / 240.0) * (up > 0.0 ? 1.0 : 0.0) * (1.0 - t);
	float glow = ring * 1.4 + flash * 0.8 + pillar * 0.55 + speck * 1.2;
	return float4(saturate(c + float3(1.0, 0.80, 0.36) * glow * worldW), 1.0);
}
