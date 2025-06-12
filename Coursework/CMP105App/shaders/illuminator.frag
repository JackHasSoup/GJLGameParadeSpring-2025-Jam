#version 130

const int MAX_LIGHTS = 15;

uniform vec2 lightPos[MAX_LIGHTS];
uniform float lightRadius[MAX_LIGHTS];
uniform vec4 lightTint[MAX_LIGHTS];
uniform float numLights;

uniform sampler1D segmentTex; //(x1, y1, x2, y2)
uniform float numSegments;

uniform vec2 screenSize;
uniform vec2 viewPos;
vec2 halfScreen = screenSize * 0.5;

vec2 screenToWorld(vec2 fragCoord) {
    fragCoord.x -= halfScreen.x;
    fragCoord.y = halfScreen.y - fragCoord.y;
    return fragCoord + viewPos;
}

vec4 getSegment(int index) {
    float u = (index + 0.5) / numSegments;
    return texture(segmentTex, u);
}

bool isOccluded(vec2 fragPos, vec2 light) {
    for (int i = 0; i < numSegments; ++i) {
        vec4 seg = getSegment(i);
        vec2 p1 = seg.xy;
        vec2 p2 = seg.zw;

        //raycast: check if light-to-fragment ray intersects segment
        vec2 r = fragPos - light;
        vec2 s = p2 - p1;

        float denom = r.x * s.y - r.y * s.x;
        if (abs(denom) < 0.0001) continue;

        vec2 diff = p1 - light;
        float t = (diff.x * s.y - diff.y * s.x) / denom;
        float u = (diff.x * r.y - diff.y * r.x) / denom;

        if (t >= 0.0 && t <= 1.0 && u >= 0.0 && u <= 1.0) {
            return true;
        }
    }
    return false;
}

void main() {
    vec2 fragPos = screenToWorld(gl_FragCoord.xy);
    vec3 colour = vec3(0.0);
    float totalIntensity = 0.0;

    for (int i = 0; i < numLights; ++i) {
        vec2 light = lightPos[i];
        float radius = lightRadius[i];
        float dist = length(fragPos - light);

        if (dist > radius) continue;

        float modif = isOccluded(fragPos, light) ? 0.35 - (dist/radius) : 1.0;
        
        //soft falloff using smoothstep
        float fade = 1.0 - smoothstep(0.0, radius, dist);
        // ease further to reduce center brightness
        fade = fade * fade * modif;

        //accumulate weighted color
        colour += lightTint[i].rgb * fade;
        totalIntensity += fade;
    }

    //normalize color to avoid blowout
    if (totalIntensity > 1.0) {
        colour /= totalIntensity; //normalize the contribution
        totalIntensity = 1.0;
    }

    gl_FragColor = vec4(colour, (1.0 - totalIntensity) * 0.65);
}

