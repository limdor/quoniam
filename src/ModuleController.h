/// \file ModuleController.h
/// \class ModuleController
/// \author Xavier Bonaventura
/// \author Copyright: (c) Universitat de Girona

#ifndef _MODULE_CONTROLLER_H_
#define _MODULE_CONTROLLER_H_

//Qt includes
#include <QMenu>
#include <QWidget>

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
