/// \file Geometry.h
/// \class Geometry
/// \author Xavier Bonaventura
/// \author Copyright: (c) Universitat de Girona

#ifndef GEOMETRY_TOPOLOGY_H
#define GEOMETRY_TOPOLOGY_H

//GLEW has to be included before any OpenGL include
#include "glew.h"

/// Enumeration to define the topology of the geometry
enum class GeometryTopology
{
    Points = GL_POINTS,
    Lines = GL_LINES,
    Line_Strip = GL_LINE_STRIP,
    Line_Loop = GL_LINE_LOOP,
    Triangles = GL_TRIANGLES
};

#endif
