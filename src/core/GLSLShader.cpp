#include "GLSLShader.h"

#include "Debug.h"

#include <QtCore/QFile>
#include <QtCore/QString>
#include <QtCore/QTextStream>

GLSLShader::GLSLShader(const QString& pSourceFile, GLenum pType):
    mType(pType)
{
    // Load shader's source text.
    QFile file(pSourceFile);
    if (file.open(QIODevice::ReadOnly))
    {
        QTextStream stream (&file);
        const QString text = stream.readAll();
        file.close();

        const QByteArray code = text.toLocal8Bit();

        // Create the shader from a text file.
        mId = glCreateShader(pType);
        if( mId != 0 )
        {
            GLint blen = 0;
            GLsizei slen = 0;

            const char *constCode = code.data();
            glShaderSource( mId, 1, &constCode, nullptr );
            glCompileShader(mId);
            CHECK_GL_ERROR();
            glGetShaderiv(mId, GL_COMPILE_STATUS, &blen);
            CHECK_GL_ERROR();
            if(blen == GL_TRUE)
            {
                mHasCompilationErrors = false;
                mCompilationLog = QString("Compilation successful!");
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
                        mCompilationLog = QString::fromLocal8Bit(compilerLog);
                    }
                    else
                    {
                        mCompilationLog = QString("Could not allocate compiler log buffer!");
                    }
                }
            }
        }
        else
        {
            mHasCompilationErrors = true;
            mCompilationLog = QString("Impossible to create the shader");
        }
    }
    else
    {
        mHasCompilationErrors = true;
        mCompilationLog = QString("File not found! %1").arg(pSourceFile);
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

const QString& GLSLShader::GetCompilationLog() const
{
    return mCompilationLog;
}

void GLSLShader::ShowCompilationLog() const
{
    if(mHasCompilationErrors)
    {
        Debug::Error(mCompilationLog);
    }
    else
    {
        Debug::Log(mCompilationLog);
    }
}

GLuint GLSLShader::GetId() const
{
    return mId;
}
