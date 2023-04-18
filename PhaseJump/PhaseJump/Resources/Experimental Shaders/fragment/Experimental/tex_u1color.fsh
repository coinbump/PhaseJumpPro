/*
	Use texture's alpha to render a single color (same as interpolate 1.0, but faster).
	Useful for pattern masks, shadows.
 */

// mediump is required to support OpenGL ES 2.0+
// FUTURE: the shader compiler should prepend this when loading.
#define __PREMULT_PHASE__	1

uniform sampler2D u_texture;
uniform mediump vec4 u_color;

varying mediump vec2 v_texCoord;

void main()
{
	/*
		BLENDING: In theory, default blend should work, but it's causing problems. Low alpha turns to white.
	 
		Use kBlendShader to get the correct shader (may change in future versions).
	 
	 */
	// FUTURE: support non premultiplied texture phase if needed.
	mediump vec4 color4 = texture2D(u_texture, v_texCoord);
	mediump float alpha = color4.a * u_color.a;
	gl_FragColor = vec4(u_color.rgb*alpha, alpha);

}
