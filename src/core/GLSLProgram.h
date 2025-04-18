#ifndef _GLSL_PROGRAM_
#define _GLSL_PROGRAM_

//GLEW has to be included before any OpenGL include
#include "GL/glew.h"

#include "GLSLShader.h"

#include "glm/mat4x4.hpp"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"

#include <map>
#include <string>

/// Class to wrap the GLSL programs used by OpenGL
class GLSLProgram
{
public:
    explicit GLSLProgram(const std::string& pName);
    ~GLSLProgram();

    /// Get the name of the program
    const std::string& GetName() const;

    /// Attach the shader \param pShader to the program
    void AttachShader(const GLSLShader& pShader);
    /// Links the program
    void LinkProgram();
    /// Set the program as the current program
    void UseProgram() const;

    /// Show the information of the program like uniforms and attributes
    void ShowInformation() const;

    /// Get the location of the uniform \param pName
    GLint GetUniformLocation(const std::string& pName) const;
    /// Set the square matrix \param pValue to \param pName uniform
    void SetUniform(const std::string &pName, const glm::mat4 &pValue) const;
    /// Set the vector \param pValue to \param pName uniform
    void SetUniform(const std::string& pName, const glm::vec4& pValue) const;
    /// Set the vector \param pValue to \param pName uniform
    void SetUniform(const std::string& pName, const glm::vec3& pValue) const;
    /// Set the float \param pValue to \param pName uniform
    void SetUniform(const std::string& pName, float pValue) const;
    /// Set the int \param pValue to \param pName uniform
    void SetUniform(const std::string& pName, int pValue) const;
    /// Set the bool \param pValue to \param pName uniform
    void SetUniform(const std::string& pName, bool pValue) const;
    /// Get the location of the attribute \param pName
    GLint GetAttribLocation(const std::string& pName) const;
    /// Bind the output variable \param pName to the fragment shader color number \param pLocation
    void BindFragDataLocation(GLuint pLocation, const std::string& pName);
    /// Bind the texture \param pTextureId of type \param pTarget to the texture unit
    /// \param pTextureUnit defined as \param pTextureName in the program
    void BindTexture(GLenum pTarget, const std::string& pTextureName, GLuint pTextureId, int pTextureUnit);

private:
    /// Id of the program
    const GLuint mId;
    /// Name of the program
    const std::string mName;
    /// Hash table that associates each uniform name to the correspoinding uniform location
    std::map<std::string, GLint> mUniforms;
    /// Hash table that associates each attribute name to the correspoinding attribute location
    std::map<std::string, GLint> mAttributes;
};
#endif
