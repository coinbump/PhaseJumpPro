/*
	FRAGMENT SHADER: Texture Varying Modulate
 
	Modulate texture color by varying vertex color.
 
 */

// mediump is required to support OpenGL ES 2.0+
// FUTURE: the shader compiler should prepend this when loading.
#define __PREMULT_PHASE__	1

uniform sampler2D u_texture;

varying mediump vec4 v_color;
varying mediump vec2 v_texCoord;

void main()
{
#ifdef __PREMULT_PHASE__
	mediump vec4 intermedColor = texture2D(u_texture, v_texCoord) * v_color;
	gl_FragColor = vec4(intermedColor.rgb*v_color.a, intermedColor.a);
#else
	gl_FragColor = texture2D(u_texture, v_texCoord) * v_color;
#endif
}
