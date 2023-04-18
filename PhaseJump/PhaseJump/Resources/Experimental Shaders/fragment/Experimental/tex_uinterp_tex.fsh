/*
	Interpolate between two bound textures by alpha factor.
 */

// mediump is required to support OpenGL ES 2.0+
uniform sampler2D u_texture;
uniform sampler2D u_texture1;
uniform mediump float u_alpha;

varying mediump vec2 v_texCoord;

void main()
{
	mediump vec4 color1 = texture2D(u_texture, v_texCoord);
	mediump vec4 color2 = texture2D(u_texture1, v_texCoord);
	
	mediump vec4 mixColor = mix(color1, color2, u_alpha);
	gl_FragColor = mixColor;
}
