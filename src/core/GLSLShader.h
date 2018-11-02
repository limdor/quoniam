/// \file GLSLShader.h
/// \class GLSLShader
/// \author Xavier Bonaventura
/// \author Copyright: (c) Universitat de Girona

#ifndef _GLSL_SHADER_
#define _GLSL_SHADER_

//GLEW has to be included before any OpenGL include
#include "glew.h"

//Qt includes
#include <QString>

/// Class to wrap the GLSL shaders used by OpenGL
class GLSLShader
{
public:
    /// Constructor
    GLSLShader(const QString& pSourceFile, GLenum pType);
    /// Destructor
    ~GLSLShader();

    /// Return if the shader has had errors during compilation
    bool HasErrors() const;
    /// Get the compilation log
    const QString& GetLog() const;
    /// Show the compilation log
    void ShowLog() const;
    /// Get the shader id
    GLuint GetId() const;

private:
    /// Shader id
    GLuint mGLId;
    /// Shader type using the same values than OpenGL (GL_VERTEX_SHADER, GL_TESS_CONTROL_SHADER,
    /// GL_TESS_EVALUATION_SHADER, GL_GEOMETRY_SHADER and GL_FRAGMENT_SHADER)
    GLenum mType;
    /// Compilation log
    QString mLog;
    /// Boolean to know if the compilation process has produced some errors
    bool mHasErrors;
};

#endif
