
/*
	Discards fragments for stencil (allows non-alpha texture pixels to determine stencil).
	OPTIMIZE: avoid discard unless needed (if statement in shader is inefficient)
	
	REF: http://stackoverflow.com/questions/8509051/is-discard-bad-for-program-performance-in-opengl
 */
// mediump is required to support OpenGL ES 2.0+
uniform sampler2D u_texture;
varying mediump vec2 v_texCoord;

// Copy the texture.
void main()
{
	mediump vec4 texColor = texture2D(u_texture, v_texCoord);
	
	if (texColor.a < 1.0) {
		discard;
	}
	
	gl_FragColor = texColor;
}
