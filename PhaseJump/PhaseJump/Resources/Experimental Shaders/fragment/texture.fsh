/*
	Copy the texture
 */
// highp is required to support OpenGL ES 2.0+
precision highp float;

uniform sampler2D u_texture;
varying vec2 v_texCoord;

void main()
{
	gl_FragColor = texture2D(u_texture, v_texCoord);
}
