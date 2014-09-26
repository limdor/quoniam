#version 150

#extension GL_ARB_explicit_attrib_location : enable

#define POSITION 0

layout(location = POSITION) in vec3 Position;

uniform mat4 modelViewProjection;

void main()
{
	gl_Position = modelViewProjection * vec4(Position, 1.0);
}
