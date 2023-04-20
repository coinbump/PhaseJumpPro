/*
	Invert texture
 */
in vec2 v_texCoord;

uniform sampler2D u_texture;
uniform vec4 u_color;

out vec4 fragColor;

void main()
{
	vec4 color4 = texture(u_texture, v_texCoord);
	fragColor = vec4(1.0 - color4.r, 1.0 - color4.g, 1.0 - color4.b, color4.a);
}
