/*
	FRAGMENT SHADER: Texture RGB Color
 
	Uses red, green, and alpha channels from the texture to map 3 alternate colors.
	Useful for simple color swapping.
 
	EXAMPLE: a UI element that has 3 colors: frame, center, bevel. Each can have its
	own color channel (r, g, b). Those color channels can be mapped via this shader
	to recolor the UI element dynamically.
	
 */
// mediump is required to support OpenGL ES 2.0+
uniform sampler2D u_texture;
uniform mediump vec4 u_colorR;
uniform mediump vec4 u_colorG;
uniform mediump vec4 u_colorB;

varying mediump vec2 v_texCoord;

void main()
{
	mediump vec4 texColor = texture2D(u_texture, v_texCoord);
	mediump vec4 r = u_colorR * texColor.r;
	mediump vec4 g = u_colorG * texColor.g;
	mediump vec4 b = u_colorB * texColor.b;
	
	mediump vec4 finalColor;
	finalColor.r = r.r+g.r+b.r;
	finalColor.g = r.g+g.g+b.g;
	finalColor.b = r.b+g.b+b.b;
	finalColor.a = texColor.a;
	finalColor *= u_colorR.a;
	
	gl_FragColor = finalColor;
}
