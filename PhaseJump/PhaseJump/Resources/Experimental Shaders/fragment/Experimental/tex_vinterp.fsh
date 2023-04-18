/*
	FRAGMENT SHADER: Texture Varying Interpolate
 
	Interpolate texture by varying vertex color.
 
 */

// mediump is required to support OpenGL ES 2.0+
// FUTURE: the shader compiler should prepend this when loading.
#define __PREMULT_PHASE__	1

uniform sampler2D u_texture;

varying mediump vec2 v_texCoord;
varying mediump vec4 v_color;
varying mediump vec4 v_alpha;

void main()
{
#ifdef __PREMULT_PHASE__
	mediump vec4 tex4Color = texture2D(v_texture, v_texCoord);
	mediump vec4 mixColor = mix(tex4Color, v_color, v_alpha);
	gl_FragColor = vec4(mixColor.rgb*tex4Color.a, v_color.a*tex4Color.a);
#else
	mediump vec4 tex4Color = texture2D(v_texture, v_texCoord);
	mediump vec4 mixColor = mix(tex4Color, v_color, v_alpha);
	gl_FragColor = vec4(mixColor.rgb, v_color.a*tex4Color.a);
#endif
}
