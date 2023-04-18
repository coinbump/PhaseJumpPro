/*
	Desaturate texture colors by uniform alpha factor.
 */
// is required to support OpenGL ES 2.0+
// FUTURE: the shader compiler should prepend this when loading.
#define _PREMULT_PHASE_	1

precision highp float;

uniform sampler2D u_texture;
uniform float u_alpha;	// Desaturation factor

varying vec2 v_texCoord;

void main()
{
	vec4 tex4Color = texture2D(u_texture, v_texCoord);
	float desatColor = (tex4Color.r*.3+tex4Color.g*.59+tex4Color.b*.11);
	vec4 mixColor = mix(tex4Color, vec4(desatColor, desatColor, desatColor, 1.0), u_alpha);
	
	gl_FragColor = vec4(mixColor.rgb*tex4Color.a, mixColor.a*tex4Color.a);
}
