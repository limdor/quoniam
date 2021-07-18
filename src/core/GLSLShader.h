#ifndef _GLSL_SHADER_
#define _GLSL_SHADER_

//GLEW has to be included before any OpenGL include
#include "glew.h"

#include <QtCore/QString>

/// Class to wrap the GLSL shaders used by OpenGL
class GLSLShader
{
public:
    GLSLShader(const QString& pSourceFile, GLenum pType);
    ~GLSLShader();

    bool HasCompilationErrors() const;
    const QString& GetCompilationLog() const;
    void ShowCompilationLog() const;
    GLuint GetId() const;

private:
    GLuint mId;
    /// Shader type using the same values than OpenGL (GL_VERTEX_SHADER, GL_TESS_CONTROL_SHADER,
    /// GL_TESS_EVALUATION_SHADER, GL_GEOMETRY_SHADER and GL_FRAGMENT_SHADER)
    GLenum mType;
    QString mCompilationLog;
    bool mHasCompilationErrors;
};

#endif
