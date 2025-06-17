#version 120
uniform sampler2D texture;
uniform float timerMax;
uniform float timer;

float clamp(float x, float minVal, float maxVal){
return min(max(x, minVal), maxVal);
}


void main()
{
    // lookup the pixel in the texture
    vec2 uv = gl_TexCoord[0].xy;
    vec4 pixel = texture2D(texture, uv);

    float timeFactor = mix(0.0,1.0,(timer/timerMax)*2);
    timeFactor = clamp(timeFactor, 0.0, 1.0);

    vec4 flashColor = vec4(0.68, 0.04, 0.11,pixel.a);

    vec4 whiteFlashColor = vec4(1.0,1.0,1.0,pixel.a);

    float tenthFactor = ((timer - (timerMax / 2)) / timerMax)*5; 
    tenthFactor = clamp(tenthFactor,0.0,1.0);

    vec4 combinedColor = mix(flashColor, whiteFlashColor, tenthFactor*2);

    gl_FragColor = mix(pixel, combinedColor, timeFactor);

}