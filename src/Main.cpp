#ifdef _DEBUG
#include "vld.h"
#endif

//Qt includes
#include <QApplication>

//Project includes
#include "MainWindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    MainWindow* window = MainWindow::GetInstance();

    window->setWindowState(Qt::WindowMaximized);
    window->setAttribute(Qt::WA_DeleteOnClose);
    window->show();

    return app.exec();
}
