#ifndef MODULE_TAB_WIDGET_H
#define MODULE_TAB_WIDGET_H

//Qt includes
#include <QtCore/QMap>
#include <QtWidgets/QTabWidget>

/// Class to handle the different moudles that you can load in the program using different tabs
class ModuleTabWidget : public QTabWidget
{
    Q_OBJECT
public:
    explicit ModuleTabWidget(QWidget *pParent = 0);
    ~ModuleTabWidget();

    void LoadModule(QWidget *pModule, const QString &pCaption, const QString &pModuleIdentifier);
    void RemoveModule(QWidget *pModule);

    QMap<QWidget*, QString> GetLoadedModules() const;

private:
    void CreateConnections();

    QMap<QWidget*, QString> mLoadedModules;

private slots:
    void CloseModuleByTabIndex(int pIndex);
    
};

#endif
