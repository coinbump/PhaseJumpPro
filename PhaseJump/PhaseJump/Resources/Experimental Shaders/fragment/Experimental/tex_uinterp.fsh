/*
	Interpolate texture by a uniform color.
 */

// mediump is required to support OpenGL ES 2.0+
// FUTURE: the shader compiler should prepend this when loading.
#define __PREMULT_PHASE__	1

uniform sampler2D u_texture;
uniform mediump vec4 u_color;
uniform mediump float u_alpha;

varying mediump vec2 v_texCoord;

void main()
{
	mediump vec4 tex4Color = texture2D(u_texture, v_texCoord);

#ifdef __PREMULT_PHASE__
	mediump vec4 mixColor = mix(tex4Color, u_color, u_alpha);
	mediump float mixAlpha = tex4Color.a*mixColor.a;	// Must multiply all channels.
	gl_FragColor = vec4(mixColor.rgb*mixAlpha, mixAlpha);
#else
	mediump vec4 mixColor = mix(tex4Color, u_color, u_alpha);
	gl_FragColor = vec4(mixColor.rgb, mixColor.a*tex4Color.a);
#endif

}
