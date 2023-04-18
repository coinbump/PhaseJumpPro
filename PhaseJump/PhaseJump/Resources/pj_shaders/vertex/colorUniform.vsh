in vec4 a_position;

//uniform mat4 u_modelViewProjMatrix;

/*
Uniform color across vertices
*/
void main()
{
	gl_Position = a_position;
}
