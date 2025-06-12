#version 120
const int MAX_LIGHTS = 15;

uniform vec2 lightPos[MAX_LIGHTS];
uniform float lightRadius[MAX_LIGHTS];
//uniform vec4 lightTint[MAX_LIGHTS];
uniform float numLights=1.0; //there may not be the max number of lights, stop when reach end of how many there really are
uniform mat4 modelMatrix;
uniform mat4 projectionViewMatrix;
uniform vec2 centrePos;

varying vec2 wp;
varying vec2 lp;

void main()
{	
	vec4 worldPos = modelMatrix * gl_Vertex;
	wp = worldPos.xy;

	vec2 finalPos = worldPos.xy;
	for(int i = 0; i < numLights; i++)
	{
		vec2 lightToVertex = worldPos.xy - lightPos[i];
		float dist = length(lightToVertex);

		if (dist < lightRadius[i]) {
			vec2 dir = lightToVertex/dist;
			vec2 lightToCentre = centrePos - lightPos[i];

			vec2 perp = vec2(lightToCentre.y, lightToCentre.x);
			float side = dot(dir, normalize(perp));
			gl_FrontColor = vec4(side,side,side,1.0);
			//facing < 0, facing away form light (in shadow), extrude
			if(side > 0.0)
			{
				float stretchAmount = clamp(lightRadius[i] - dist, 0.0, lightRadius[i]);
				finalPos += dir * stretchAmount;
			}

			/*
			float facing = dot(dir, lightToCentre);
			gl_FrontColor = vec4(facing,facing,facing,1.0);
			//facing < 0, facing away form light (in shadow), extrude
			if(facing < 0.0 || true)
			{
				float stretchAmount = clamp(lightRadius[i] - dist, 0.0, lightRadius[i]);
				finalPos += dir * stretchAmount;
			}
			*/
		}
	}
	
	gl_Position = projectionViewMatrix * vec4(finalPos, 0.0, 1.0);
}