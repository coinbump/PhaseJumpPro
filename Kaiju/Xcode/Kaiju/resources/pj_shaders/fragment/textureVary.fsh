/*
	Modulate texture by varying color
 */
in vec2 v_texCoord;
in vec4 v_color;

uniform sampler2D u_texture;

out vec4 fragColor;

void main()
{
#ifdef _PREMULT_PHASE_
	// Premultiplied modulation:
	vec4 intermedColor = texture(u_texture, v_texCoord) * v_color;
	fragColor = vec4(intermedColor.rgb*v_color.a, intermedColor.a);
#else
	fragColor = texture(u_texture, v_texCoord) * v_color;
#endif
}
