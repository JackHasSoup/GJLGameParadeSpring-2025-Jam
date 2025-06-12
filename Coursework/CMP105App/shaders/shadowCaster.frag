#version 120
const int MAX_LIGHTS = 15;

uniform vec2 lightPos[MAX_LIGHTS];
uniform float lightRadius[MAX_LIGHTS];
uniform vec4 lightTint[MAX_LIGHTS];
uniform float numLights; //there may not be the max number of lights, stop when reach end of how many there really are
uniform mat4 modelMatrix;
uniform mat4 projectionViewMatrix;

varying vec2 wp;
varying vec2 lp;

void main()
{
	float a = length((wp+gl_TexCoord[0].xy) - lp);
	gl_FragColor = gl_Color;//vec4(100.0 * (1.0/(abs(wp.x - lp.x))),100.0*(1.0/(abs(wp.y - lp.y))),0.0, 1.0);
}