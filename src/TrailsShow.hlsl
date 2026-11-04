// TrailsShow.hlsl -- (new by th4) SPELL TRAILS, pass 2 of 2: lays the trail buffer (Trails.hlsl) over
// the frame. Driver: dx_utility.cpp Trails_Present. "Lighten": a pixel shows the brighter of itself
// and the afterimage, so a trail never darkens anything and the spell itself is drawn unchanged.
// WORLD pixels only (soft mask as Mist.hlsl).
// s0 = frame copy (point, clamp), s1 = world/UI mask (linear), s2 = trail buffer (point, clamp)
sampler2D frame : register(s0);
sampler2D mask : register(s1);
sampler2D trail : register(s2);

float4 main_trailsshow(float2 uv : TEXCOORD0) : COLOR
{
	float3 c = tex2D(frame, uv).rgb;
	float worldW = saturate(tex2D(mask, uv).r * (255.0 / 80.0));
	return float4(lerp(c, max(c, tex2D(trail, uv).rgb), worldW), 1.0);
}
