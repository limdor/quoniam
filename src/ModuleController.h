/// \file ModuleController.h
/// \class ModuleController
/// \author Xavier Bonaventura
/// \author Copyright: (c) Universitat de Girona

#ifndef MODULE_CONTROLLER_H
#define MODULE_CONTROLLER_H

//Qt includes
#include <QtWidgets/QMenu>
#include <QtWidgets/QWidget>

class ModuleController : public QWidget
{
    Q_OBJECT
public:
    explicit ModuleController(QWidget *pParent = 0);
    ~ModuleController();
    
    QVector<QMenu*> GetModuleMenus() const;
    virtual void CreateModuleMenus();

    virtual void ActiveModule();

protected:
    QVector<QMenu*> mMenus;

};

#endif
