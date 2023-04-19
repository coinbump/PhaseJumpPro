in vec4 a_position;

uniform mat4 u_mvpMatrix;

/*
Uniform color across vertices
*/
void main()
{
	gl_Position = u_mvpMatrix * a_position;
}
