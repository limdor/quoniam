/// \file GLSLProgram.h
/// \class GLSLProgram
/// \author Xavier Bonaventura
/// \author Copyright: (c) Universitat de Girona

#ifndef _GLSL_PROGRAM_
#define _GLSL_PROGRAM_

//GLEW has to be included before any OpenGL include
#include "glew.h"

//Qt includes
#include <QHash>
#include <QString>
#include <QVector>

//Dependency includes
#include "glm/mat4x4.hpp"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"

//Project includes
#include "GLSLShader.h"

/// Class to wrap the GLSL programs used by OpenGL
class GLSLProgram
{
public:
    /// Constructor
    explicit GLSLProgram(const QString& pName);
    /// Destructor
    ~GLSLProgram();

    /// Get the name of the program
    QString GetName() const;

    /// Attach the shader \param pShader to the program
    void AttachShader(GLSLShader* pShader);
    /// Links the program
    void LinkProgram();
    /// Set the program as the current program
    void UseProgram() const;

    /// Show the information of the program like uniforms and attributes
    void ShowInformation() const;

    /// Get the location of the uniform \param pName
    GLint GetUniformLocation(const QString& pName) const;
    /// Set the square matrix \param pValue to \param pName uniform
    void SetUniform(const QString &pName, const glm::mat4 &pValue) const;
    /// Set the vector \param pValue to \param pName uniform
    void SetUniform(const QString& pName, const glm::vec4& pValue) const;
    /// Set the vector \param pValue to \param pName uniform
    void SetUniform(const QString& pName, const glm::vec3& pValue) const;
    /// Set the float \param pValue to \param pName uniform
    void SetUniform(const QString& pName, float pValue) const;
    /// Set the int \param pValue to \param pName uniform
    void SetUniform(const QString& pName, int pValue) const;
    /// Set the bool \param pValue to \param pName uniform
    void SetUniform(const QString& pName, bool pValue) const;
    /// Get the location of the attribute \param pName
    GLint GetAttribLocation(const QString& pName) const;
    /// Bind the output variable \param pName to the fragment shader color number \param pLocation
    void BindFragDataLocation(GLuint pLocation, const QString& pName);
    /// Bind the texture \param pTextureId of type \param pTarget to the texture unit
    /// \param pTextureUnit defined as \param pTextureName in the program
    void BindTexture(GLenum pTarget, const QString& pTextureName, GLuint pTextureId, int pTextureUnit);

private:
    /// Id of the program
    GLuint mId;
    /// Name of the program
    QString mName;
    /// List of shaders used by the program
    QVector<GLSLShader *> mShaders;
    /// Hash table that associates each uniform name to the correspoinding uniform location
    QHash<QString, GLint> mUniforms;
    /// Hash table that associates each attribute name to the correspoinding attribute location
    QHash<QString, GLint> mAttributes;
};
#endif
