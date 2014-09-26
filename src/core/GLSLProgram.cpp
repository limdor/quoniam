//Definition include
#include "GLSLProgram.h"

//Dependency includes
#include "glm/gtc/type_ptr.hpp"

//Project includes
#include "Debug.h"

GLSLProgram::GLSLProgram(const QString& pName):
    mName(pName)
{
    mId = glCreateProgram();
    CHECK_GL_ERROR();
}

GLSLProgram::~GLSLProgram()
{
    glDeleteProgram(mId);
}

QString GLSLProgram::GetName() const
{
    return mName;
}

void GLSLProgram::AttachShader(GLSLShader* pShader)
{
    GLuint shaderId = pShader->GetId();
    glAttachShader(mId, shaderId);
    CHECK_GL_ERROR();
}

void GLSLProgram::LinkProgram()
{
    glLinkProgram(mId);
    CHECK_GL_ERROR();

    // Get the location of every uniform
    mUniforms.clear();
    GLint nUniforms, maxLen, nAttributes;
    glGetProgramiv( mId, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxLen);
    glGetProgramiv( mId, GL_ACTIVE_UNIFORMS, &nUniforms);
    GLchar * name = (GLchar *) malloc( maxLen );
    for( int i = 0; i < nUniforms; ++i )
    {
        GLint size;
        GLenum type;

        glGetActiveUniform( mId, i, maxLen, NULL, &size, &type, name );
        GLint location = glGetUniformLocation(mId, name);
        mUniforms[QString(name)] = location;
    }
    free(name);

    // Get the location of every attribute
    mAttributes.clear();
    glGetProgramiv( mId, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &maxLen);
    glGetProgramiv( mId, GL_ACTIVE_ATTRIBUTES, &nAttributes);
    name = (GLchar *) malloc( maxLen );
    for( int i = 0; i < nAttributes; ++i )
    {
        GLint size;
        GLenum type;

        glGetActiveAttrib( mId, i, maxLen, NULL, &size, &type, name );
        GLint location = glGetAttribLocation(mId, name);
        mAttributes[QString(name)] = location;
    }
    free(name);
}

void GLSLProgram::UseProgram() const
{
    glUseProgram(mId);
}

void GLSLProgram::ShowInformation() const
{
    // Show program name
    Debug::Log( QString("Program name: %1").arg(mName) );

    // Show uniforms information
    Debug::Log("Uniforms:");
    Debug::Log(" Name | Location");
    Debug::Log("------------------------------------------------");
    QHash<QString, GLint>::const_iterator it;
    for( it = mUniforms.begin(); it != mUniforms.end(); ++it )
    {
        Debug::Log( QString(" %1 | %2").arg(it.key()).arg(it.value()) );
    }

    // Show attributes information
    Debug::Log("Attributes:");
    Debug::Log(" Name | Location");
    Debug::Log("------------------------------------------------");
    for( it = mAttributes.begin(); it != mAttributes.end(); ++it )
    {
        Debug::Log( QString(" %1 | %2").arg(it.key()).arg(it.value()) );
    }
}

GLint GLSLProgram::GetUniformLocation(const QString& pName) const
{
    GLint location = mUniforms.value(pName, -1);
    if( location == -1 )
    {
        Debug::Warning( QString("Invalid uniform: %1").arg(pName) );
    }
    return location;
}

void GLSLProgram::SetUniform(const QString &pName, const glm::mat4 &pValue) const
{
    glUniformMatrix4fv( GetUniformLocation(pName), 1, GL_FALSE, glm::value_ptr(pValue));
}

void GLSLProgram::SetUniform(const QString& pName, const glm::vec4& pValue) const
{
    glUniform4fv( GetUniformLocation(pName), 1, glm::value_ptr(pValue) );
}

void GLSLProgram::SetUniform(const QString& pName, const glm::vec3& pValue) const
{
    glUniform3fv( GetUniformLocation(pName), 1, glm::value_ptr(pValue) );
}

void GLSLProgram::SetUniform(const QString& pName, float pValue) const
{
    glUniform1f( GetUniformLocation(pName), pValue );
}

void GLSLProgram::SetUniform(const QString& pName, int pValue) const
{
    glUniform1i( GetUniformLocation(pName), pValue );
}

void GLSLProgram::SetUniform(const QString& pName, bool pValue) const
{
    glUniform1i( GetUniformLocation(pName), pValue );
}

GLint GLSLProgram::GetAttribLocation(const QString& pName) const
{
    GLint location = mAttributes.value(pName, -1);
    if( location == -1 )
    {
        Debug::Warning( QString("Invalid attribute: %1").arg(pName) );
    }
    return location;
}

void GLSLProgram::BindFragDataLocation(GLuint pLocation, const QString& pName)
{
    QByteArray bAName = pName.toLocal8Bit();
    const char *constName = bAName.data();
    glBindFragDataLocation( mId, pLocation, constName );
    CHECK_GL_ERROR();
}

void GLSLProgram::BindTexture(GLenum pTarget, const QString& pTextureName, GLuint pTextureId, int pTextureUnit)
{
    glActiveTexture(GL_TEXTURE0 + pTextureUnit);
    glBindTexture(pTarget, pTextureId);
    GLint id = GetUniformLocation( pTextureName );
    if( id == -1 )
    {
        Debug::Warning( QString("Invalid texture: %1").arg(pTextureName) );
    }
    glUniform1i(id, pTextureUnit);
}
