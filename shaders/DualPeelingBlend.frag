#version 150

uniform sampler2DRect TempTex;

void main(void)
{
    gl_FragColor = texture(TempTex, gl_FragCoord.xy);
    // for occlusion query
    if (gl_FragColor.a == 0) discard;
}
