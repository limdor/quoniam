//Definition include
#include "MainWindow.h"

//Qt includes
#include <QtWidgets/QMessageBox>

//Project includes
#include "Debug.h"
#include "MainModuleController.h"

MainWindow* MainWindow::mInstance;

MainWindow* MainWindow::GetInstance()
{
    if(mInstance == nullptr)
    {
        mInstance = new MainWindow();
    }
    return mInstance;
}

MainWindow::MainWindow(QWidget *pParent): QMainWindow(pParent),
    mUi(new Ui::MainWindow)
{
    mUi->setupUi(this);

    mModuleTabWidget = mUi->moduleTabWidget;
    connect(mModuleTabWidget, SIGNAL(currentChanged(int)), this, SLOT(ModuleChanged(int)));

    LoadMainModule();
}

MainWindow::~MainWindow()
{
    delete mUi;
    delete mModuleTabWidget;
}

void MainWindow::ModuleChanged(int pIndex)
{
    Debug::SetConsole(nullptr);
    menuBar()->clear();
    if(pIndex >= 0)
    {
        // We load the menus of the current module
        ModuleController* currentModule = dynamic_cast<ModuleController*>(mModuleTabWidget->currentWidget());

        currentModule->ActiveModule();

        std::vector<QMenu*> menus = currentModule->GetModuleMenus();
        for( int i = 0; i < menus.size(); i++ )
        {
            menuBar()->addMenu(menus.at(i));
        }
    }

    // We load the menus of the main application
    QMenu* menuLoadModule = menuBar()->addMenu("Window");

    QAction* actionMainModule = new QAction("New tab", this);
    actionMainModule->setShortcut(Qt::CTRL + Qt::Key_T);
    menuLoadModule->addAction(actionMainModule);
    connect(actionMainModule, SIGNAL(triggered()), this, SLOT(LoadMainModule()));

    QMenu* menuHelp = menuBar()->addMenu("&Help");

    QAction* actionAbout = new QAction("&About", this);
    actionAbout->setShortcut(Qt::Key_F1);
    menuHelp->addAction(actionAbout);
    connect(actionAbout, SIGNAL(triggered()), this, SLOT(ShowAbout()));
}

void MainWindow::LoadMainModule()
{
    mModuleTabWidget->LoadModule(new MainModuleController(mModuleTabWidget), "Main module", "MainModule");
}

void MainWindow::ShowAbout()
{
    QMessageBox::about(this, tr("About Quoniam v%1").arg(APP_VERSION),
                       tr("<b>Quoniam</b> is an application of viewpoint selection and "
                          "object exploration developed by the University of Girona."
                          "<br><br>More information: xavierb@imae.udg.edu"));
}
