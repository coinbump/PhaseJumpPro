/*
	Invert texture colors, but not the alpha. 
 */
// mediump is required to support OpenGL ES 2.0+
uniform sampler2D u_texture;

varying mediump vec2 v_texCoord;

void main()
{
	mediump vec4 color4 = texture2D(u_texture, v_texCoord);
	gl_FragColor = vec4(1.0-color4.r, 1.0-color4.g, 1.0-color4.b, color4.a);
}
