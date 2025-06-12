#version 120
//Rerference: https://github.com/GarrettGunnell/AcerolaFX/blob/main/Shaders/AcerolaFX_ASCII.fx
//After watching Acerola's youtube video on his ascii shader I thought it was really cool, so decided to try make it
//My shader is based off of his one and uses his ascii LUT
uniform sampler2D texture;

uniform vec2 res;
uniform float exposure = 2.0;
uniform float attenuation = 2.4;
uniform bool u_invertLuminance = false;

uniform sampler2D ASCIILookup;
uniform vec2 ASCIILUTSize = vec2(160.0, 8.0);
uniform vec4 ASCIIColour = vec4(1.0,1.0,1.0,1.0);
uniform vec4 bgColour = vec4(0.0,0.0,0.0,0.05);
uniform float blend = 1.0;

float blockSize = ASCIILUTSize.y; //minimum of ASCIILUTSize.y or ascii isn't aligned to same grid as pixelisation (or don't blend bg with base)

float easeLuminance(float l)
{
    return 1.0 - sqrt(1.0 - (l*l));
}

float getLuminance(vec3 colour)
{
    return dot(colour, vec3(0.299, 0.587, 0.114));
}

void main() 
{
    //determine pos in blockSize grid (blockSize = one char)
    vec2 pixelPos = gl_TexCoord[0].xy * res;
    vec2 blockUV = mod(pixelPos, blockSize);
    vec2 blockIndex = floor(pixelPos / blockSize);

    //scale down render
    vec2 pixelBlockSize = blockSize / res;

    //move uv to topleft of block
    vec2 snappedUV = floor(gl_TexCoord[0].xy / pixelBlockSize) * pixelBlockSize;
    vec4 colour = texture2D(texture, snappedUV);
    float luminance = getLuminance(colour.rgb);

    //change lumincance
    luminance = pow(luminance * exposure, attenuation);
    if (u_invertLuminance) luminance = 1.0 - luminance;
    luminance = clamp(floor(luminance * 10.0 - 1.0) / 10.0, 0.0, 1.0);

    float x = mod(blockUV.x + luminance * ASCIILUTSize.x, ASCIILUTSize.x);
    float y = blockUV.y;
    vec2 asciiUV = vec2(mod(x, ASCIILUTSize.x) / ASCIILUTSize.x,  1.0 - (y / ASCIILUTSize.y)); //y coord 1- to fix y flipped chars

    float asciiChar = texture2D(ASCIILookup, asciiUV).r;

    vec4 asciiColour = mix(bgColour, mix(ASCIIColour, colour, blend), max(asciiChar, easeLuminance(getLuminance(colour.rgb)) + bgColour.a));


    gl_FragColor = asciiColour;
}
