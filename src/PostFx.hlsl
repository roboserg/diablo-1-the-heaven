// PostFx.hlsl -- optional final screen effects, one pass over the finished frame:
//   HEAT HAZE, WATER SHIMMER, SHOCKWAVE rings, TILT-SHIFT, CHROMATIC ABERRATION, FILM GRAIN, CRT.
//
// Runs LAST, on the finished window-size frame in the back buffer (after the game quad,
// the true-colour overlays and any FSR / SharpScaling upscale). See dx_utility.cpp
// PostFxPresent. Config (all default off; "on" = 5, or 1..10): HeatHaze, WaterShimmer,
// Shockwave, TiltShift, ChromaticAberration, FilmGrain, CRT.
//
// WORLD MASK (s1, L8, half game resolution, built on the CPU by dx_utility.cpp HeatHaze_BuildMask,
// only when an effect that needs it is on):
//   0   = not world (UI, text, cursor -- anything drawn over the world): never distorted/blurred
//   80  = world pixel
//   170 = world pixel showing water (cave water maps)
//   255 = world pixel showing burning lava
// Haze: a pixel shimmers when lava lies BELOW it on screen (heat rises), fading with distance.
// Water: the water surface itself ripples; nothing outside it moves.
//
// Constants:
//   c0.xy = 1 / frame size   c0.z = aberration in pixels at the corners   c0.w = grain amount
//   c1.x  = frame counter (grain)   c1.y = haze amplitude in pixels (0 = off)   c1.z = time in s
//   c2.xy = mask texel size in uv (one texel = 2x2 game px)   c2.z = 1 / the DYNAMIC ZOOM factor (1 = none)
//   c3..c6 = shockwave rings: xy = centre (uv), z = radius (uv, screen-height units), w = strength
//   c7.x = tilt-shift amount 0..1   c7.y = CRT amount 0..1   c7.z = water sway in px   c7.w = aspect w/h
//   c8.x = game frame height (CRT scanlines follow game pixel rows)   c8.y = 1 if the mask is valid
//   c8.zw = the DYNAMIC ZOOM shift (game px): drawn frame = (screen - c8.zw) * c2.z (dx_utility.cpp THE VIEW)
//   c9.xy = position (game px) of world tile (0,0) in the DRAWN frame   c9.z/w = Perspective coefficients a/b
sampler src  : register(s0);
sampler heat : register(s1);
float4 con0 : register(c0);
float4 con1 : register(c1);
float4 con2 : register(c2);
float4 ring[4] : register(c3);
float4 con7 : register(c7);
float4 con8 : register(c8);
float4 con9 : register(c9);

float Mask(float2 uv) { return tex2Dlod(heat, float4(uv, 0, 0)).r; }

float4 main_postfx(float2 vpos : VPOS) : COLOR
{
	float2 uv = (vpos + 0.5) * con0.xy;
	const bool maskOn = con8.y > 0.5;
	float here = maskOn ? Mask(uv) : 0.3;			// no mask: treat everything as world
	const bool world = here > 0.15;

	// ---------------- heat haze (lava) ----------------
	[branch] if( con1.y > 0.0 && world ){
		// heat below this pixel: 12 mask taps downwards, every 2 game pixels (24 px), nearer
		// counts more. dist = how far above the nearest lava pixel we are.
		float h = 0.0, dist = 99.0;
		[loop] for( int k = 0; k < 12; ++k ){
			float m = Mask(uv + float2(0.0, k * con2.y));	// one mask texel = 2 game px
			float hot = m > 0.85 ? 1.0 : 0.0;
			h += hot * (1.0 - k / 12.0);
			dist = min(dist, hot > 0.0 ? k * 2.0 : 99.0);
		}
		// inside a lava area dist is 0 everywhere; give it depth by how far the lava reaches
		// upwards from here, so the surface ripples instead of swaying as one block
		if( here > 0.85 ){
			float up = 0.0;
			[loop] for( int j = 1; j <= 8; ++j ) up += Mask(uv - float2(0.0, j * con2.y)) > 0.85 ? 2.0 : 0.0;
			dist = -up;
		}
		h = saturate(h / 4.0);
		// The phase is measured from the LAVA, not the screen, so camera scrolling does not
		// change the apparent speed; only time drives it.
		float t = con1.z;
		float wave = sin(dist * 0.30 - t * 4.0) * 0.6 + sin(dist * 0.11 - t * 2.3) * 0.4;
		float2 moved = uv;
		moved.x += wave * h * con1.y * con0.x;
		moved.y += sin(dist * 0.21 - t * 3.0) * h * con1.y * 0.35 * con0.y;
		// never PULL colour from UI (letters, panels): keep still if the displaced spot is UI
		if( Mask(moved) > 0.15 ) uv = moved;
	}

	// ---------------- water shimmer ----------------
	[branch] if( con7.z > 0.0 && here > 0.5 && here < 0.85 ){
		// The ripple's phase comes from the WORLD position of the pixel, not from the screen or
		// the mask: screen-anchored phases slide while scrolling, and mask-anchored ones jump
		// whenever something (cursor, a walking monster) covers part of the water.
		float2 gsz = 2.0 / con2.xy;						// game frame size in px
		float2 p = (uv * gsz - con8.zw) * con2.z;		// (th4) undo the DYNAMIC ZOOM first: it is applied after the warp
		if( con9.z > 0.0 || con9.w > 0.0 ){				// undo Perspective (Draw.cpp Perspective_ScreenToWorld)
			float u = p.y / gsz.y;
			p.y = gsz.y * (u + con9.z * u * (1.0 - u));
			p.x = gsz.x * 0.5 + (p.x - gsz.x * 0.5) / (1.0 + con9.w * u);
		}
		float2 wp = p - con9.xy;						// fixed for a given spot of water
		float t = con1.z;
		float2 moved = uv;
		moved.x += (sin(wp.y * 0.21 + t * 1.7) * 0.6 + sin((wp.x * 0.5 + wp.y) * 0.09 - t * 1.1) * 0.4) * con7.z * con0.x;
		moved.y += sin(wp.x * 0.11 + wp.y * 0.07 + t * 1.3) * con7.z * 0.5 * con0.y;
		float mm = Mask(moved);
		if( mm > 0.5 && mm < 0.85 ) uv = moved;	// only sample water from water: shores stay clean
	}

	// ---------------- shockwave rings ----------------
	[branch] if( maskOn && world ){
		float2 total = 0;
		[unroll] for( int i = 0; i < 4; ++i ){			// unrolled: ps_3_0 cannot index constants dynamically
			float2 dv = uv - ring[i].xy;
			dv.x *= con7.w;								// measure in screen-height units (round rings)
			float dl = max(length(dv), 1e-4);
			float band = 1.0 - saturate(abs(dl - ring[i].z) / 0.035);	// ring width
			float2 dir = dv / dl; dir.x /= con7.w;
			total += dir * band * ring[i].w;			// w = 0 for unused rings
		}
		float2 moved = uv - total;
		if( Mask(moved) > 0.15 ) uv = moved;
	}

	// ---------------- colour: tilt-shift blur (world only) + chromatic aberration ----------------
	float2 d = uv - 0.5;
	float r2 = dot(d, d) * 2.0;						// 0 at the centre, 1 at the corners
	float len = max(length(d), 1e-5);
	float2 off = (d / len) * r2 * con0.z * con0.xy;	// radial red/blue split, in uv
	float3 c;
	float blur = 0.0;
	[branch] if( con7.x > 0.0 && world ){
		// sharp band around the upper-middle (where the hero stands), blurring toward top/bottom
		blur = con7.x * saturate((abs(uv.y - 0.45) - 0.14) / 0.30);
	}
	[branch] if( blur > 0.01 ){
		// 8-tap ring, radius up to 6 game px; UI taps are skipped so panels never bleed in
		float3 acc = float3(tex2Dlod(src, float4(uv + off, 0, 0)).r, tex2Dlod(src, float4(uv, 0, 0)).g, tex2Dlod(src, float4(uv - off, 0, 0)).b);
		float wsum = 1.0;
		[loop] for( int k = 0; k < 8; ++k ){
			float2 sc; sincos(k * 0.785398, sc.y, sc.x);	// direction of tap k (no constant arrays in ps_3_0 loops)
			float2 p = uv + sc * blur * 6.0 * con2.xy * 0.5;
			if( Mask(p) > 0.15 ){
				acc += float3(tex2Dlod(src, float4(p + off, 0, 0)).r, tex2Dlod(src, float4(p, 0, 0)).g, tex2Dlod(src, float4(p - off, 0, 0)).b);
				wsum += 1.0;
			}
		}
		c = acc / wsum;
	}else{
		c.r = tex2Dlod(src, float4(uv + off, 0, 0)).r;
		c.g = tex2Dlod(src, float4(uv, 0, 0)).g;
		c.b = tex2Dlod(src, float4(uv - off, 0, 0)).b;
	}

	// ---------------- film grain ----------------
	// hash noise, re-seeded every frame; slightly stronger in the midtones like real film
	float n = frac(sin(dot(vpos + con1.x * float2(17.13, 31.71), float2(12.9898, 78.233))) * 43758.5453) - 0.5;
	float l = dot(c, float3(0.299, 0.587, 0.114));
	c += n * con0.w * (1.0 - 0.5 * abs(l * 2.0 - 1.0));

	// ---------------- CRT: scanlines on game pixel rows, aperture-grille mask, edge falloff ----------------
	[branch] if( con7.y > 0.0 ){
		float row = frac((vpos.y + 0.5) * con0.y * con8.x);		// position inside the game pixel row
		float scan = 1.0 - con7.y * 0.35 * (1.0 - sin(row * 3.14159));	// dark gap between rows
		float3 grille = float3(1.0, 1.0, 1.0);
		int col = (int)fmod(vpos.x, 3.0);
		float g = con7.y * 0.12;
		grille = col == 0 ? float3(1.0 + g, 1.0 - g, 1.0 - g) : (col == 1 ? float3(1.0 - g, 1.0 + g, 1.0 - g) : float3(1.0 - g, 1.0 - g, 1.0 + g));
		c *= scan * grille * (1.0 - con7.y * 0.18 * r2);
		c *= 1.0 + con7.y * 0.12;								// give back the brightness the lines take
	}
	return float4(saturate(c), 1.0);
}
