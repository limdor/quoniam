#ifdef _DEBUG
#include "vld.h"
#endif

//Qt includes
#include <QtWidgets/QApplication>

//Project includes
#include "MainWindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setAttribute(Qt::AA_UseDesktopOpenGL);

    MainWindow* window = MainWindow::GetInstance();

    window->setWindowState(Qt::WindowMaximized);
    window->setAttribute(Qt::WA_DeleteOnClose);
    window->show();

    return app.exec();
}
