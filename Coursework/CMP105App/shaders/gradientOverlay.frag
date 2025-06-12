#version 120
uniform vec4 topRight;
uniform vec4 bottomLeft;
uniform float opacity;
uniform sampler2D texture;

vec4 lerp(vec4 a, vec4 b, float factor)
{
	return a + (b-a) * factor;
}

void main()
{
	vec2 uv = gl_TexCoord[0].xy;
	vec4 texColour = texture2D(texture, uv);

	vec4 trG = lerp(topRight, vec4(0.0), uv.y);
	vec4 blG = lerp(bottomLeft, vec4(0.0), 1.0 - uv.y);
	vec4 grad = lerp(trG, blG, uv.x);

	gl_FragColor = lerp(texColour, grad, opacity);
}
