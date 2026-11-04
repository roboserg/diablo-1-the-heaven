// Trails.hlsl -- (new by th4) SPELL TRAILS, pass 1 of 2: updates the TRAIL BUFFER (config SpellTrails
// 0..10 = how long the trails last). Driver: dx_utility.cpp Trails_Present (ATMOSPHERE block);
// pass 2 is TrailsShow.hlsl. The buffer is a window-size texture kept from frame to frame
// (ping-pong pair TrailTex), holding the fading afterimages:
//   new = max( old moved with the world x decay - 2/255,  emit )
// "Moved with the world": the old buffer is sampled shifted by how far the ground scrolled since
// the last frame (c0.xy, from the tile (0,0) anchor), so an afterimage stays where the spell WAS on
// the ground while the hero walks; static bright things (torches) just refresh themselves and
// never smear. The - 2/255 lets an 8-bit buffer fade to true black (x decay alone stalls at 1..3).
// EMIT = this frame's pixel, only near a live missile (c2..c25, screen spots of Missiles[]), only
// if bright enough, and only on plain world (the mask: not UI, not lava 255, not water 170, whose
// animated colours would glow forever). So arrows and spells leave trails, the hero and monsters
// walking around do not.
//
// s0 = frame copy (point, clamp), s1 = world/UI mask (linear), s2 = old trail buffer (linear, clamp)
// c0 = xy shift of the ground since the last frame in uv, z decay per frame, w emit threshold
// c1 = xy uv per game px, z missile count (0..24), w spot radius in game px
// c2..c25 = missile spots, xy uv
sampler2D frame : register(s0);
sampler2D mask : register(s1);
sampler2D trail : register(s2);
float4 t0 : register(c0);
float4 t1 : register(c1);
float4 spots[24] : register(c2);

float4 main_trails(float2 uv : TEXCOORD0) : COLOR
{
	float2 puv = uv - t0.xy;
	float3 prev = 0;
	if (all(puv >= 0.0) && all(puv <= 1.0)) prev = max(tex2D(trail, puv).rgb * t0.z - 2.0 / 255.0, 0.0);
	float m = tex2D(mask, uv).r;
	float plain = saturate(m * (255.0 / 80.0)) * (m < 0.5 ? 1.0 : 0.0);	// world, not lava / water
	float near = 0;
	[loop] for (int i = 0; i < 24; i++) {
		if (i >= t1.z) break;
		float2 d = (uv - spots[i].xy) / t1.xy;
		if (dot(d, d) < t1.w * t1.w) near = 1.0;
	}
	float3 c = tex2D(frame, uv).rgb;
	float e = near * plain * saturate((max(c.r, max(c.g, c.b)) - t0.w) / 0.25);
	return float4(max(prev, c * e), 1.0);
}
