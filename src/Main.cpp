#ifdef _DEBUG
#include "vld.h"
#endif

//Qt includes
#include <QtWidgets/QApplication>
#include <QtGui/QIcon>
#include <QtGui/QWindow>

//Project includes
#include "MainWindow.h"

int main(int argc, char *argv[])
{
    QApplication::setAttribute(Qt::AA_UseDesktopOpenGL);
    QApplication app(argc, argv);

    MainWindow* window = MainWindow::GetInstance();
    const auto top_level_windows = QGuiApplication::topLevelWindows();
    for(auto& top_level_window : top_level_windows){
        top_level_window->setIcon(QIcon("icon.ico"));
    }

    window->setWindowState(Qt::WindowMaximized);
    window->setAttribute(Qt::WA_DeleteOnClose);
    window->show();

    return app.exec();
}
