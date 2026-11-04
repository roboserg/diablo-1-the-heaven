// FXAAShader.hlsl — luminance-based edge anti-aliasing
//
// Samples the final rendered colour (palette lookup + brightness) and
// detects edges by comparing luminance across a 3×3 neighbourhood.
// Blends ACROSS edges for anti-aliasing; does NOT brighten edge pixels
// or darken their neighbours.  Uses final-colour luminance rather than
// raw palette index so that data artifacts (stray bright/dark palette
// entries in dungeon CEL tiles) are not falsely detected as edges.
sampler tx : register(s0);
sampler pl : register(s1);

//static const float LIGHT_SCALE = 255.0 / 15.0;
// A byte is full 0-255 range: 0=full bright, 255=pitch black.
// GPU normalizes: samp.a = A_byte/255 → brightness = saturate(1.0 - samp.a).
// 256 distinct brightness levels eliminate color banding and sawtooth artifacts.
static const float LIGHT_SCALE = 1.0; // A byte range is 0-255

// Helper: sample A8L8 texture at coord, look up palette, apply brightness
// .r still gives palette index / 255 in A8L8
float4 sampleLit(float2 coord)
{
	float4 samp = tex2D(tx, coord);
	float4 c = tex1D(pl, samp.r * (255./256) + (0.5/256));
	c.rgb *= saturate(1.0 - samp.a * LIGHT_SCALE);
	return c;
}

// ITU-R BT.709 luminance
float luma(float3 c)
{
	return dot(c, float3(0.2126, 0.7152, 0.0722));
}

struct PSInput {
	float2 cr : TEXCOORD0;
};

float4 main_fxaa(PSInput i) : COLOR
{
	// Texel size via screen-space derivatives — robust regardless of
	// vertex UV setup, viewport offset, or fullscreen-quad convention.
	float w = abs(ddx(i.cr.x));
	float h = abs(ddy(i.cr.y));

	// Neighbour samples clamped to [0,1] so we never read outside the
	// texture at screen edges (where the offset would produce negative
	// or >1 coordinates, wrapping via D3D default WRAP addressing).
	float4 px   = sampleLit(i.cr);
	float4 nU   = sampleLit(saturate(i.cr + float2(  0, -h)));
	float4 nD   = sampleLit(saturate(i.cr + float2(  0,  h)));
	float4 nL   = sampleLit(saturate(i.cr + float2( -w,  0)));
	float4 nR   = sampleLit(saturate(i.cr + float2(  w,  0)));

	float lC = luma(px.rgb);
	float lU = luma(nU.rgb);
	float lD = luma(nD.rgb);
	float lL = luma(nL.rgb);
	float lR = luma(nR.rgb);

	// Luminance range in the 5-pixel cross — a proxy for local contrast.
	// Skip FXAA when contrast is very low (flat-colour area) to avoid
	// blurring texture detail and amplifying single-pixel data artifacts.
	float lMin = min(min(min(lC, lU), min(lD, lL)), lR);
	float lMax = max(max(max(lC, lU), max(lD, lL)), lR);
	float range = lMax - lMin;
	if (range < 0.0625) // 16/256 — roughly one palette band step
		return px;

	// Estimate edge direction from horizontal / vertical second differences.
	// hGrad large → sharp change left↔right → edge runs VERTICALLY.
	// vGrad large → sharp change up↔down    → edge runs HORIZONTALLY.
	// To anti-alias, blend ACROSS the edge (perpendicular), not along it.
	float hGrad = abs(lL + lR - 2.0 * lC);
	float vGrad = abs(lU + lD - 2.0 * lC);

	// Blend factor: stronger edges get more blending.  Clamped to avoid
	// over-blurring.
	float factor = saturate(2.0 * max(hGrad, vGrad));

	if (hGrad > vGrad)
	{
		// Vertical edge — blend horizontally (across left/right boundary)
		float4 blended = lerp(px, (nL + px + nR) / 3.0, factor * 0.45);
		return float4(blended.rgb, 1.0);
	}
	else
	{
		// Horizontal edge — blend vertically (across top/bottom boundary)
		float4 blended = lerp(px, (nU + px + nD) / 3.0, factor * 0.45);
		return float4(blended.rgb, 1.0);
	}
}