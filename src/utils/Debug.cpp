//GLEW has to be included before any OpenGL include
#include "glew.h"

#include "Debug.h"

QPlainTextEdit* Debug::mConsole;

void Debug::Log(const QString &pMessage)
{
    qDebug(pMessage.toStdString().c_str());
}

void Debug::Warning(const QString& pMessage)
{
    qWarning(pMessage.toStdString().c_str());
}

void Debug::Error(const QString &pMessage)
{
    qCritical(pMessage.toStdString().c_str());
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

bool Debug::CheckGLError(const char *pFile, int pLine)
{
    auto const errors = ExtractGlErrorsFromDriver();
    bool const retCode = !errors.empty();
    for (auto const &error : errors)
    {
        auto const &[error_code, optional_string] = error;
        QString const optional_message{optional_string ? QString::fromStdString(*optional_string) : "no message available"};
        Debug::Error(QString("GL Error #%1(%2) in file %3 at line: %4").arg(error_code).arg(optional_message).arg(pFile).arg(pLine));
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
