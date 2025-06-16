#version 120
uniform float index;
uniform float currentHealth;
uniform float hitTimer;
uniform sampler2D texture;

void main()
{
    // lookup the pixel in the texture
    vec2 uv = gl_TexCoord[0].xy;
    vec4 pixel = texture2D(texture, uv);

    float isEmpty = step(0.0,(currentHealth - (index + 0.5)));

    float isOnIndex = 0.0;
    if( currentHealth - fract(currentHealth) == index){
    isOnIndex = 1;
    }

    float currentTimer = lerp(0.0,1.0,hitTimer * 10.0);

    float halfMask = isOnIndex * step(0.5,uv.x) * isEmpty;

    float xMask = (1 - (step(0.5, fract(currentHealth)) * halfMask)) * isEmpty;

    vec4 emptyColor = vec4(0.22, 0.32, 0.5, 1.0);

    // multiply it by the color
    vec4 pixColor = mix(pixel * emptyColor, pixel, xMask);
    pixColor.a = pixel.a;

    vec4 flashColor = mix(vec4(0.39, 0.11, 0.14,1.0),vec4(1.0, 1.0, 1.0,1.0),xMask);

    vec4 endColor = mix(pixColor, (pixColor + flashColor), max(0.0, currentTimer));
    endColor.a = pixel.a;

    gl_FragColor = endColor;
}