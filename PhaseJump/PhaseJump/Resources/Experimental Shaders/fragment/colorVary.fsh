/*
	Vertex Colors specify interpolated color for fragment
 */
// highp is required to support OpenGL ES 2.0+
precision highp float;

varying vec4 v_color;

void main()
{
	gl_FragColor = v_color;
}
