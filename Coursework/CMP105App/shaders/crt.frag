uniform sampler2D texture;  
uniform float time; 
uniform float scanlineCount;

const float scanlineIntensity = 0.25;
const float distortion = 0.005;//screen curvature
const float chromaDistortion = 0.0005; //chromatic aberration strength
const float flickerIntensity = 0.0015;
const float noiseIntensity = 0.035; 
const float brightnessFactor = 1.1; 

float frequency = 0.6;
float amplitude = 0.025;

float random(vec2 pos) {
    return fract(sin(dot(pos, vec2(12.9898, 78.233))) * 43758.5453);
}

void main() {
    vec2 uv = gl_TexCoord[0].xy;
    
    //screen curvature
    vec2 centeredUV = uv * 2.0 - 1.0;
    float distSqr = centeredUV.x * centeredUV.x + centeredUV.y * centeredUV.y;
    vec2 distortedUV = uv + centeredUV * (distSqr * distortion);
    
    //check if the distorted coordinates are outside of the texture
    if (distortedUV.x < 0.0 || distortedUV.x > 1.0 || 
        distortedUV.y < 0.0 || distortedUV.y > 1.0) {
        gl_FragColor = vec4(0.0, 0.0, 0.0, 1.0);
        return;
    }
    
    //chromatic aberration
    vec2 redOffset = vec2(chromaDistortion, 0.0);
    vec2 greenOffset = vec2(0.0, 0.0);
    vec2 blueOffset = vec2(-chromaDistortion, 0.0);
    
    float red = texture2D(texture, distortedUV + redOffset).r;
    float green = texture2D(texture, distortedUV + greenOffset).g;
    float blue = texture2D(texture, distortedUV + blueOffset).b;
    
    //combine RGB channels
    vec4 texColour = vec4(red, green, blue, 1.0);
    float scanlineEffect = 1.0 - scanlineIntensity * sin(distortedUV.y * scanlineCount * 3.14159);
    float flicker = 1.0 - flickerIntensity * (0.5 + 0.5 * sin(time * 8.0));
    float noise = 1.0 - noiseIntensity * random(uv + vec2(time * 0.01));
    
    texColour.rgb *= scanlineEffect  * flicker * noise * brightnessFactor;
    
    gl_FragColor = texColour * gl_Color;
}