#version 120
uniform sampler2D texture;

void main()
{
	vec2 uv = gl_TexCoord[0].xy;
	vec4 texColour = texture2D(texture, uv);

	gl_FragColor = vec4(1.0,0.0,1.0,1.0);
}