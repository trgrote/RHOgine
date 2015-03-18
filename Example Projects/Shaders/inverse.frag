#version 110

uniform sampler2D texture;

void main(void)
{

	// lookup the pixel in the texture
    vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);
	gl_FragColor.rgb = 1.0 - pixel.rgb;
	gl_FragColor.a = pixel.a;
}
