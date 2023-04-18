/*
	FRAGMENT SHADER: Texture Negative Stencil (DEPRECATED)
 
	Defines a negative stencil via texture.
 
	DEPRECATED: use stencil buffer instead.
 
 */
// mediump is required to support OpenGL ES 2.0+
uniform sampler2D u_texture;
uniform sampler2D u_texture1;

varying mediump vec2 v_texCoord;

void main()
{
	mediump vec4 color1 = texture2D(u_texture, v_texCoord);
	mediump vec4 color2 = texture2D(u_texture1, v_texCoord);
	
	gl_FragColor = color1 * (1.0-color2.r);
}
