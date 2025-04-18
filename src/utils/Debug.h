#ifndef DEBUG_H
#define DEBUG_H

#include <GL/glew.h>

#include <QtWidgets/QPlainTextEdit>

#include <vector>
#include <optional>
#include <tuple>
#include <string>

/// Class to output log, warning and error messages through a console
class Debug
{
public:
    /// Output a log message
    static void Log(const std::string& pMessage);
    /// Output a warning message
    static void Warning(const std::string& pMessage);
    /// Output a error message
    static void Error(const std::string& pMessage);

    /// Set the console
    static void SetConsole(QPlainTextEdit * pConsole);

    static std::vector<std::tuple<GLenum, std::optional<std::string>>> ExtractGlErrorsFromDriver();

    /// If there are errors return true and show them using Debug::Error
    static bool CheckGLError(std::string const& pFile, int pLine);
#ifdef QT_DEBUG
    #define CHECK_GL_ERROR() Debug::CheckGLError(__FILE__, __LINE__)
#else
    #define CHECK_GL_ERROR()
#endif
private:
    static void ConsoleOutput(QtMsgType pType, const QMessageLogContext &, const QString &pMessage);

    /// Console
    static QPlainTextEdit* mConsole;

};
#endif
