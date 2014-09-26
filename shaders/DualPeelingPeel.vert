#version 150

#extension GL_ARB_explicit_attrib_location : enable

#define POSITION  0
#define NORMAL    1
#define COLOR     2
#define TEXTCOORD 3

layout(location = POSITION)  in vec3  vi_PositionOS;
layout(location = NORMAL)    in vec3    vi_NormalOS;
layout(location = COLOR)     in vec4       vi_Color;
layout(location = TEXTCOORD) in vec2   vi_TextCoord;

out vec3         vo_NormalWS; // Vector normal en espai de món
out vec3       vo_PositionWS; // Posició en espai de món
out vec4            vo_Color; // Color per vertex
out vec2        vo_TextCoord; // Coordenades de textura

uniform mat4 modelViewProjection; // World view projection matrix
uniform mat4               world; // World matrix

void main(void)
{
    gl_Position = modelViewProjection *  vec4(vi_PositionOS, 1.0);

    vo_NormalWS = mat3(world) * vi_NormalOS;
    vo_PositionWS = mat3(world) * vi_PositionOS;
    vo_Color = vi_Color;
    vo_TextCoord = vi_TextCoord;
}
