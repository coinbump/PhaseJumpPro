attribute vec4 a_position;
attribute vec4 a_color;

uniform mat4 u_modelViewProjMatrix;

varying vec4 v_color;

/*
Interpolates color across vertices
*/
void main()
{
	gl_Position = u_modelViewProjMatrix * a_position;
	v_color = a_color;
}
