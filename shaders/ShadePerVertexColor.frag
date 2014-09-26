#version 150

uniform float alpha;

in vec4    vo_Color; // Color per vertex

vec4 ShadeFragment()
{
    return vo_Color;
}
