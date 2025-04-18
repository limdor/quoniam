//GLEW has to be included before any OpenGL include
#include "GL/glew.h"

#include "Debug.h"

QPlainTextEdit* Debug::mConsole;

void Debug::Log(const std::string &pMessage)
{
    qDebug(pMessage.c_str());
}

void Debug::Warning(const std::string& pMessage)
{
    qWarning(pMessage.c_str());
}

void Debug::Error(const std::string &pMessage)
{
    qCritical(pMessage.c_str());
}

void Debug::SetConsole(QPlainTextEdit * pConsole)
{
    mConsole = pConsole;

    if(mConsole)
    {
        qInstallMessageHandler(Debug::ConsoleOutput);
        QPalette p = mConsole->palette();
        p.setColor(QPalette::Base, Qt::lightGray);
        mConsole->setPalette(p);
    }
    else
    {
        qInstallMessageHandler(0);
    }

}

std::string iso_8859_1_to_utf8(std::string const &str)
{
    std::string strOut;
    for (unsigned char const ch : str)
    {
        if (ch < 0x80) {
            strOut.push_back(ch);
        }
        else {
            strOut.push_back(0xc0 | ch >> 6);
            strOut.push_back(0x80 | (ch & 0x3f));
        }
    }
    return strOut;
}

bool Debug::CheckGLError(std::string const& pFile, int pLine)
{
    auto const errors = ExtractGlErrorsFromDriver();
    bool const retCode = !errors.empty();
    for (auto const &error : errors)
    {
        auto const &[error_code, optional_string] = error;
        std::string const optional_message{optional_string ? iso_8859_1_to_utf8(*optional_string) : "no message available"};
        Debug::Error("GL Error #" + std::to_string(error_code) + "(" + optional_message + ") in file " + pFile + " at line: " + std::to_string(pLine));
    }
    return retCode;
}

std::vector<std::tuple<GLenum, std::optional<std::string>>> Debug::ExtractGlErrorsFromDriver()
{
    std::vector<std::tuple<GLenum, std::optional<std::string>>> errors;
    GLenum glErr = glGetError();
    while (glErr != GL_NO_ERROR)
    {
        GLubyte const *const sError = gluErrorString(glErr);
        auto error_message = sError ? std::optional<std::string>{reinterpret_cast<char const *>(sError)} : std::optional<std::string>{};
        auto element = std::make_tuple(glErr, std::move(error_message));
        errors.push_back(std::move(element));
        glErr = glGetError();
    }
    return errors;
}

void Debug::ConsoleOutput(QtMsgType pType, const QMessageLogContext &, const QString &pMessage)
{
    switch(pType)
    {
        case QtInfoMsg:
        case QtDebugMsg:
            mConsole->appendHtml( QString("<FONT color=black>%1</FONT>").arg( QString(pMessage).replace(" ","&nbsp;") ) );
            fprintf(stdout, "Log: %s\n", pMessage.toLocal8Bit().constData());
            fflush(stdout);
            break;
        case QtWarningMsg:
            mConsole->appendHtml( QString("<FONT color=yellow>%1</FONT>").arg( QString(pMessage).replace(" ","&nbsp;") ) );
            fprintf(stderr, "Warning: %s\n", pMessage.toLocal8Bit().constData());
            fflush(stderr);
            break;
        case QtCriticalMsg:
        case QtFatalMsg:
            mConsole->appendHtml( QString("<FONT color=red>%1</FONT>").arg( QString(pMessage).replace(" ","&nbsp;") ) );
            fprintf(stderr, "Error: %s\n", pMessage.toLocal8Bit().constData());
            fflush(stderr);
            break;
    }
}
