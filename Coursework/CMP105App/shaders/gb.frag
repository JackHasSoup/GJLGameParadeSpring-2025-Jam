#version 120
uniform sampler2D texture;

//REFERENCE: https://easings.net/#easeOutQuint
float easeOut(float x)
{
    return x*x;
}

void main()
{
//list of colours for indexing
    vec3 colours[4] = vec3[](
    vec3(.2, .1725, .3137),
    vec3(.2745, .5294, .5607),
    vec3(.5803, .8901, .2666),
    vec3(.8862, .9529, .8941)
    );

    //get the tex colour, then sum the channels for brightness
    vec4 tex = texture2D(texture, gl_TexCoord[0].xy);
    float total = tex.r + tex.g + tex.b;
    
    //get percentage from max brightness
    total /= 3.0;

    //map brightness to index
    int i = int(ceil(easeOut(total) * 4.0));
    i = int(clamp(i, 0, 3));

    gl_FragColor = gl_Color * vec4(colours[i], 1.0);
}