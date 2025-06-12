#version 120

const int MAX_LIGHTS = 15;
const int MAX_SEGMENTS = 256;

uniform vec2 lightPos[MAX_LIGHTS];
uniform float lightRadius[MAX_LIGHTS];
uniform float numLights;

uniform vec4 segments[MAX_SEGMENTS]; //(x1, y1, x2, y2)
uniform float numSegments;
varying vec2 worldPos;

void main()
{
    worldPos = (gl_ModelViewProjectionMatrix * gl_Vertex).xy;
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}
