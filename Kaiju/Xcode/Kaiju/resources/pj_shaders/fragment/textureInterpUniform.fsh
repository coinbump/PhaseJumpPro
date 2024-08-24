/*
	Interpret texture towards uniform color
 */
in vec2 v_texCoord;

// TODO: deprecate this shader. uniform colors can't be batched
uniform sampler2D u_texture;
uniform vec4 u_color;
uniform float u_float;

out vec4 fragColor;

void main()
{
    vec4 texColor = texture(u_texture, v_texCoord);

#ifdef _PREMULT_PHASE_
    vec4 mixColor = mix(texColor, u_color, u_float);
    float mixAlpha = texColor.a * mixColor.a;    // Multiply all channels
    fragColor = vec4(mixColor.rgb * mixAlpha, mixAlpha);
#else
    vec4 mixColor = mix(texColor, u_color, u_float);
    fragColor = vec4(mixColor.rgb, mixColor.a * texColor.a);
#endif
}
