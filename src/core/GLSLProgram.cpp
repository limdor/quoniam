#include "GLSLProgram.h"

#include "Debug.h"

#include "glm/gtc/type_ptr.hpp"

GLSLProgram::GLSLProgram(const std::string& pName):
    mName(pName),
    mId(glCreateProgram())
{
    CHECK_GL_ERROR();
}

GLSLProgram::~GLSLProgram()
{
    glDeleteProgram(mId);
}

const std::string& GLSLProgram::GetName() const
{
    return mName;
}

void GLSLProgram::AttachShader(const GLSLShader& pShader)
{
    glAttachShader(mId, pShader.GetId());
    CHECK_GL_ERROR();
}

void GLSLProgram::LinkProgram()
{
    glLinkProgram(mId);
    CHECK_GL_ERROR();

    // Get the location of every uniform
    mUniforms.clear();
    GLint nUniforms, maxLen;
    glGetProgramiv( mId, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxLen);
    glGetProgramiv( mId, GL_ACTIVE_UNIFORMS, &nUniforms);
    std::vector<GLchar> name;
    name.reserve(maxLen);
    for( GLint i = 0; i < nUniforms; ++i )
    {
        GLint size;
        GLenum type;

        glGetActiveUniform( mId, i, maxLen, nullptr, &size, &type, name.data() );
        GLint location = glGetUniformLocation(mId, name.data());
        mUniforms[std::string(name.data())] = location;
    }

    // Get the location of every attribute
    mAttributes.clear();
    glGetProgramiv( mId, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &maxLen);
    GLint nAttributes;
    glGetProgramiv( mId, GL_ACTIVE_ATTRIBUTES, &nAttributes);
    name.reserve(maxLen);
    for( GLint i = 0; i < nAttributes; ++i )
    {
        GLint size;
        GLenum type;

        glGetActiveAttrib( mId, i, maxLen, nullptr, &size, &type, name.data() );
        GLint location = glGetAttribLocation(mId, name.data());
        mAttributes[std::string(name.data())] = location;
    }
}

void GLSLProgram::UseProgram() const
{
    glUseProgram(mId);
}

void GLSLProgram::ShowInformation() const
{
    // Show program name
    Debug::Log( "Program name: " + mName );

    // Show uniforms information
    Debug::Log("Uniforms:");
    Debug::Log(" Name | Location");
    Debug::Log("------------------------------------------------");;
    for( auto it = mUniforms.cbegin(); it != mUniforms.cend(); ++it )
    {
        Debug::Log( " " + it->first + " | " + std::to_string(it->second));
    }

    // Show attributes information
    Debug::Log("Attributes:");
    Debug::Log(" Name | Location");
    Debug::Log("------------------------------------------------");
    for( auto it = mAttributes.cbegin(); it != mAttributes.cend(); ++it )
    {
        Debug::Log( " " + it->first + " | " + std::to_string(it->second));
    }
}

GLint GLSLProgram::GetUniformLocation(const std::string& pName) const
{
    const auto it = mUniforms.find(pName);
    if( it == mUniforms.end() )
    {
        Debug::Warning( "Invalid uniform: " + pName);
        return -1;
    }
    else
    {
        return it->second;
    }
}

void GLSLProgram::SetUniform(const std::string &pName, const glm::mat4 &pValue) const
{
    glUniformMatrix4fv( GetUniformLocation(pName), 1, GL_FALSE, glm::value_ptr(pValue));
}

void GLSLProgram::SetUniform(const std::string& pName, const glm::vec4& pValue) const
{
    glUniform4fv( GetUniformLocation(pName), 1, glm::value_ptr(pValue) );
}

void GLSLProgram::SetUniform(const std::string& pName, const glm::vec3& pValue) const
{
    glUniform3fv( GetUniformLocation(pName), 1, glm::value_ptr(pValue) );
}

void GLSLProgram::SetUniform(const std::string& pName, float pValue) const
{
    glUniform1f( GetUniformLocation(pName), pValue );
}

void GLSLProgram::SetUniform(const std::string& pName, int pValue) const
{
    glUniform1i( GetUniformLocation(pName), pValue );
}

void GLSLProgram::SetUniform(const std::string& pName, bool pValue) const
{
    glUniform1i( GetUniformLocation(pName), pValue );
}

GLint GLSLProgram::GetAttribLocation(const std::string& pName) const
{
    const auto it = mAttributes.find(pName);
    if( it == mAttributes.end() )
    {
        Debug::Warning( "Invalid attribute: " + pName );
        return -1;
    }
    else
    {
        return it->second;
    }
}

void GLSLProgram::BindFragDataLocation(GLuint pLocation, const std::string& pName)
{
    const char *constName = pName.c_str();
    glBindFragDataLocation( mId, pLocation, constName );
    CHECK_GL_ERROR();
}

void GLSLProgram::BindTexture(GLenum pTarget, const std::string& pTextureName, GLuint pTextureId, int pTextureUnit)
{
    glActiveTexture(GL_TEXTURE0 + pTextureUnit);
    glBindTexture(pTarget, pTextureId);
    GLint id = GetUniformLocation( pTextureName );
    if( id == -1 )
    {
        Debug::Warning( "Invalid texture: " + pTextureName );
    }
    glUniform1i(id, pTextureUnit);
}
