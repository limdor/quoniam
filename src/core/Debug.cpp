//Definition include
#include "Debug.h"

//GLEW has to be included before any OpenGL include
#include "glew.h"

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
#if QT_VERSION < 0x50000
        qInstallMsgHandler(Debug::ConsoleOutput);
#else
        qInstallMessageHandler(Debug::ConsoleOutput);
#endif
        QPalette p = mConsole->palette();
        p.setColor(QPalette::Base, Qt::lightGray);
        mConsole->setPalette(p);
    }
    else
    {
#if QT_VERSION < 0x50000
        qInstallMsgHandler(0);
#else
        qInstallMessageHandler(0);
#endif
    }

}

bool Debug::CheckGLError(const char *pFile, int pLine)
{
    bool retCode = false;
    GLenum glErr = glGetError();
    while (glErr != GL_NO_ERROR)
    {
        const GLubyte* sError = gluErrorString(glErr);

        if (sError)
            Debug::Error( QString("GL Error #%1(%2) in file %3 at line: %4").arg(glErr).arg( QString((const char*)sError) ).arg(pFile).arg(pLine) );
        else
            Debug::Error( QString("GL Error #%1(no message available) in file %2 at line: %3").arg(glErr).arg(pFile).arg(pLine) );

        retCode = true;
        glErr = glGetError();
    }
    return retCode;
}
#if QT_VERSION < 0x50000
void Debug::ConsoleOutput(QtMsgType pType, const char *pMessage)
{
    switch(pType)
    {
        case QtDebugMsg:
            mConsole->appendHtml( QString("<FONT color=black>%1</FONT>").arg( QString(pMessage).replace(" ","&nbsp;") ) );
            fprintf(stdout, "Log: %s\n", pMessage);
            fflush(stdout);
            break;
        case QtWarningMsg:
            mConsole->appendHtml( QString("<FONT color=yellow>%1</FONT>").arg( QString(pMessage).replace(" ","&nbsp;") ) );
            fprintf(stderr, "Warning: %s\n", pMessage);
            fflush(stderr);
            break;
        case QtCriticalMsg:
            mConsole->appendHtml( QString("<FONT color=red>%1</FONT>").arg( QString(pMessage).replace(" ","&nbsp;") ) );
            fprintf(stderr, "Error: %s\n", pMessage);
            fflush(stderr);
            break;
    }
}
#else
void Debug::ConsoleOutput(QtMsgType pType, const QMessageLogContext &, const QString &pMessage)
{
    switch(pType)
    {
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
            mConsole->appendHtml( QString("<FONT color=red>%1</FONT>").arg( QString(pMessage).replace(" ","&nbsp;") ) );
            fprintf(stderr, "Error: %s\n", pMessage.toLocal8Bit().constData());
            fflush(stderr);
            break;
    }
}
#endif
