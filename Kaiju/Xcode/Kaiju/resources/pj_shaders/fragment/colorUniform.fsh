/*
	Render uniform color

    DEPRECATED: This shader is for testing the render engine
    Avoid it because it can't be batched
 */
uniform vec4 u_color;

out vec4 fragColor;

void main()
{
	fragColor = u_color;	
}
