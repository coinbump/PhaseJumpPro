attribute vec4 a_position;
attribute vec2 a_texCoord;
attribute vec4 a_color;

uniform mat4 u_modelViewProjMatrix;

varying vec2 v_texCoord;
varying vec4 v_color;

/*
Vary vertex color with texture
*/
void main()
{
	gl_Position = u_modelViewProjMatrix * a_position;
	v_texCoord = a_texCoord;
	v_color = a_color;
}
