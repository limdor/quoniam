#include "ModuleController.h"

ModuleController::ModuleController(QWidget *pParent): QWidget(pParent)
{

}

ModuleController::~ModuleController()
{

}

QVector<QMenu*> ModuleController::GetModuleMenus() const
{
    return mMenus;
}

void ModuleController::CreateModuleMenus()
{

}

void ModuleController::ActiveModule()
{

}
