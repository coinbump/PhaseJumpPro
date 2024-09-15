in vec4 a_position;
in vec2 a_texCoord;

uniform mat4 u_mvpMatrix;

out vec2 v_texCoord;

/*
Uniform color modulated texture
*/
void main()
{
	gl_Position = u_mvpMatrix * a_position;
    v_texCoord = a_texCoord;
}
