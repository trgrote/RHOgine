#version 110

uniform sampler2D texture;

void main(void)
{
	// lookup the pixel in the texture
    vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);
	
	// float gray = dot( gl_Color.rgb, vec3( 0.3, 0.59, 0.11 ) ) ;
	float gray = dot( pixel.rgb, vec3( 0.3, 0.59, 0.11 ) ) ;
	
	gl_FragColor = vec4( gray, gray, gray, pixel.a );
}
