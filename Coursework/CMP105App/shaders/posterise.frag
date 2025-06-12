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
    vec3 colours[8] = vec3[](
    vec3(.0509, .1686, .2705),
    vec3(.1254, .2352, .3372),
    vec3(.3294, .3058, .4078),
    vec3(.5529, .4117, .4784),
    vec3(.8156, .5058, .3490),
    vec3(1.0  , .6666, .3686),
    vec3(1.0  , .8313, .6392),
    vec3(1.0  , .9254, .8392)
    );

    //get the tex colour, then sum the channels for brightness
    vec4 tex = texture2D(texture, gl_TexCoord[0].xy);
    float total = tex.r + tex.g + tex.b;
    
    //get percentage from max brightness
    total /= 3.0;

    //map brightness to index
    int i = int(ceil(easeOut(total) * 8.0));
    i = int(clamp(i, 0, 7));

    gl_FragColor = gl_Color * vec4(colours[i], 1.0);
}