
/*
	The texture defines a gradient map from u_color to u_color1 using red channel.
	Texture should be from black (u_color) to white (u_color1).
 */

// mediump is required to support OpenGL ES 2.0+
// FUTURE: the shader compiler should prepend this when loading.
#define __PREMULT_PHASE__	1

uniform sampler2D u_texture;
uniform mediump vec4 u_color;
uniform mediump vec4 u_color1;

varying mediump vec2 v_texCoord;

void main()
{
	mediump vec4 texColor = texture2D(u_texture, v_texCoord);
	mediump vec4 mixColor = mix(u_color, u_color1, texColor.r);
	
	// For premultiplied textures, multiply every channel.
#ifdef __PREMULT_PHASE__
	gl_FragColor = vec4(mixColor.rgb*texColor.a,texColor.a);
#else
	gl_FragColor = vec4(mixColor.rgb,texColor.a);
#endif
}
