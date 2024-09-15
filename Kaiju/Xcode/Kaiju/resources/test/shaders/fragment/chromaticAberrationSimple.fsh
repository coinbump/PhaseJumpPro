in vec2 v_texCoord;
in vec4 v_color;

uniform sampler2D u_texture;

out vec4 fragColor;

#define redOffset vec2(-0.01, 0.)
#define greenOffset vec2(0.)
#define blueOffset vec2(0.01, 0.)

void main()
{
    vec4 rValue = texture(u_texture, v_texCoord + redOffset);
    vec4 gValue = texture(u_texture, v_texCoord + greenOffset);
    vec4 bValue = texture(u_texture, v_texCoord + blueOffset);

    // Combine the offset colors.
    fragColor = vec4(rValue.r, gValue.g, bValue.b, texture(u_texture, v_texCoord).a);
}
