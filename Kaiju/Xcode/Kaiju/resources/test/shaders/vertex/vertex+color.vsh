in vec4 a_position;
in vec4 a_color;

uniform mat4 u_mvpMatrix;

out vec4 v_color;

/*
Uniform color across vertices
*/
void main()
{
	gl_Position = u_mvpMatrix * a_position;
    v_color = a_color;
}
