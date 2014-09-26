/// \file Debug.h
/// \class Debug
/// \author Xavier Bonaventura
/// \author Copyright: (c) Universitat de Girona

#ifndef _DEBUG_H_
#define _DEBUG_H_

//Qt includes
#include <QPlainTextEdit>

/// Class to output log, warning and error messages through a console
class Debug
{
public:
    /// Output a log message
    static void Log(const QString& pMessage);
    /// Output a warning message
    static void Warning(const QString& pMessage);
    /// Output a error message
    static void Error(const QString& pMessage);

    /// Set the console
    static void SetConsole(QPlainTextEdit * pConsole);

    /// If there are errors return true and show them using Debug::Error
    static bool CheckGLError(const char *pFile, int pLine);
#ifdef QT_DEBUG
    #define CHECK_GL_ERROR() Debug::CheckGLError(__FILE__, __LINE__)
#else
    #define CHECK_GL_ERROR()
#endif
private:
    /// Message handler
#if QT_VERSION < 0x50000
    static void ConsoleOutput(QtMsgType pType, const char *pMessage);
#else
    static void ConsoleOutput(QtMsgType pType, const QMessageLogContext &, const QString &pMessage);
#endif

    /// Console
    static QPlainTextEdit* mConsole;

};
#endif
