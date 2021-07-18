#include "GLSLShader.h"

#include "Debug.h"

#include <QtCore/QString>

#include <fstream>

GLSLShader::GLSLShader(const std::string& pSourceFile, GLenum pType):
    mType(pType)
{
    // Load shader's source text.
    std::ifstream file(pSourceFile, std::ios::in);
    if (!file.is_open())
    {
        mCompilationLog = "File not found! " + pSourceFile;
        mHasCompilationErrors = true;
    }
    else
    {
        std::string const fileContent{std::istreambuf_iterator<char>{file}, {}};
        file.close();

        // Create the shader from a text file.
        mId = glCreateShader(pType);
        if( mId != 0 )
        {
            GLint blen = 0;
            GLsizei slen = 0;

            const char *constCode = fileContent.c_str();
            glShaderSource( mId, 1, &constCode, nullptr );
            glCompileShader(mId);
            CHECK_GL_ERROR();
            glGetShaderiv(mId, GL_COMPILE_STATUS, &blen);
            CHECK_GL_ERROR();
            if(blen == GL_TRUE)
            {
                mHasCompilationErrors = false;
                mCompilationLog = "Compilation successful!";
            }
            else
            {
                mHasCompilationErrors = true;
                glGetShaderiv(mId, GL_INFO_LOG_LENGTH, &blen);
                CHECK_GL_ERROR();
                if (blen > 1)
                {
                    char * compilerLog = new char[blen];
                    if ( compilerLog != nullptr )
                    {
                        glGetShaderInfoLog(mId, blen, &slen, compilerLog);
                        CHECK_GL_ERROR();
                        mCompilationLog = compilerLog;
                    }
                    else
                    {
                        mCompilationLog = "Could not allocate compiler log buffer!";
                    }
                }
            }
        }
        else
        {
            mHasCompilationErrors = true;
            mCompilationLog = "Impossible to create the shader";
        }
    }
}

GLSLShader::~GLSLShader()
{
    glDeleteShader(mId);
}

bool GLSLShader::HasCompilationErrors() const
{
    return mHasCompilationErrors;
}

const std::string& GLSLShader::GetCompilationLog() const
{
    return mCompilationLog;
}

void GLSLShader::ShowCompilationLog() const
{
    if(mHasCompilationErrors)
    {
        Debug::Error(QString::fromStdString(mCompilationLog));
    }
    else
    {
        Debug::Log(QString::fromStdString(mCompilationLog));
    }
}

GLuint GLSLShader::GetId() const
{
    return mId;
}
