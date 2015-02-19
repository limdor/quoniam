#include "ModuleTabWidget.h"

ModuleTabWidget::ModuleTabWidget(QWidget *pParent): QTabWidget(pParent)
{
    setTabsClosable(true);
    setTabPosition(QTabWidget::South);

    CreateConnections();
}

ModuleTabWidget::~ModuleTabWidget()
{
    int numberOfModules = count();
    for( int i = numberOfModules - 1; i >= 0; i-- )
    {
        RemoveModule( widget(i) );
    }
}

void ModuleTabWidget::LoadModule(QWidget *pModule, const QString &pCaption, const QString &pModuleIdentifier)
{
    // Add the module as a Tab
    int position = addTab(pModule, pCaption);
    setCurrentIndex(position);
    // Add the module to the list of the loaded modules
    mLoadedModules.insert(pModule, pModuleIdentifier);
}

void ModuleTabWidget::RemoveModule(QWidget *pModule)
{
    // Remoeve the tab of the module
    removeTab(indexOf(pModule));
    // Remove the module from the list of the loaded modules
    mLoadedModules.remove(pModule);
    // Remove the module itself
    delete pModule;
}

QMap<QWidget*, QString> ModuleTabWidget::GetLoadedModules() const
{
    return mLoadedModules;
}

void ModuleTabWidget::CreateConnections()
{
    connect(this, SIGNAL(tabCloseRequested(int)), SLOT(CloseModuleByTabIndex(int)));
}

void ModuleTabWidget::CloseModuleByTabIndex(int pIndex)
{
    //Ensure at least one module loaded
    if(mLoadedModules.size() > 1)
    {
        RemoveModule(widget(pIndex));
    }
}
