in vec4 a_position;
in vec2 a_texCoord;
in vec4 a_color;

uniform mat4 u_mvpMatrix;

out vec2 v_texCoord;
out vec4 v_color;

/*
 Standard vertex shader
 */
void main()
{
    gl_Position = u_mvpMatrix * a_position;
    v_texCoord = a_texCoord;
    v_color = a_color;
}
