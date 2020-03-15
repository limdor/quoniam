//Definition include
#include "GLSLShader.h"

//Qt includes
#include <QtCore/QFile>
#include <QtCore/QString>
#include <QtCore/QTextStream>

//Project includes
#include "Debug.h"

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
        mGLId = glCreateShader(pType);
        if( mGLId != 0 )
        {
            GLint blen = 0;
            GLsizei slen = 0;

            const char *constCode = code.data();
            glShaderSource( mGLId, 1, &constCode, nullptr );
            glCompileShader(mGLId);
            CHECK_GL_ERROR();
            glGetShaderiv(mGLId, GL_COMPILE_STATUS, &blen);
            CHECK_GL_ERROR();
            if(blen == GL_TRUE)
            {
                mHasErrors = false;
                mLog = QString("Compilation successful!");
            }
            else
            {
                mHasErrors = true;
                glGetShaderiv(mGLId, GL_INFO_LOG_LENGTH, &blen);
                CHECK_GL_ERROR();
                if (blen > 1)
                {
                    char * compilerLog = new char[blen];
                    if ( compilerLog != nullptr )
                    {
                        glGetShaderInfoLog(mGLId, blen, &slen, compilerLog);
                        CHECK_GL_ERROR();
                        mLog = QString::fromLocal8Bit(compilerLog);
                    }
                    else
                    {
                        mLog = QString("Could not allocate compiler log buffer!");
                    }
                }
            }
        }
        else
        {
            mHasErrors = true;
            mLog = QString("Impossible to create the shader");
        }
    }
    else
    {
        mHasErrors = true;
        mLog = QString("File not found! %1").arg(pSourceFile);
    }
}

GLSLShader::~GLSLShader()
{
    glDeleteShader(mGLId);
}

bool GLSLShader::HasErrors() const
{
    return mHasErrors;
}

const QString& GLSLShader::GetLog() const
{
    return mLog;
}

void GLSLShader::ShowLog() const
{
    if(mHasErrors)
    {
        Debug::Error(mLog);
    }
    else
    {
        Debug::Log(mLog);
    }
}

GLuint GLSLShader::GetId() const
{
    return mGLId;
}
