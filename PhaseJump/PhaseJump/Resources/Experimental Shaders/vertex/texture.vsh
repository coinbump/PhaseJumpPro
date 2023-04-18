attribute vec4 a_position;
attribute vec2 a_texCoord;

uniform mat4 u_modelViewProjMatrix;
varying vec2 v_texCoord;

/*
Texture pass through
*/
void main()
{
	gl_Position = u_modelViewProjMatrix * a_position;
	v_texCoord = a_texCoord;
}
