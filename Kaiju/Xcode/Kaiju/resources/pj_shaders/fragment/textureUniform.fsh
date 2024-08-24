/*
	Modulate texture by uniform color
 */
in vec2 v_texCoord;

// TODO: deprecate this shader. uniform colors can't be batched
uniform sampler2D u_texture;
uniform vec4 u_color;

out vec4 fragColor;

void main()
{
#ifdef _PREMULT_PHASE_
	// Premultiplied modulation:
	vec4 intermedColor = texture(u_texture, v_texCoord) * u_color;
	fragColor = vec4(intermedColor.rgb*u_color.a, intermedColor.a);
#else
	fragColor = texture(u_texture, v_texCoord) * u_color;
#endif
}
