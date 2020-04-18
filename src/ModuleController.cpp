#include "ModuleController.h"

ModuleController::ModuleController(QWidget *pParent): QWidget(pParent)
{

}

std::vector<QMenu*> ModuleController::GetModuleMenus() const
{
    return mMenus;
}

void ModuleController::CreateModuleMenus()
{

}

void ModuleController::ActiveModule()
{

}
