#ifndef _MAIN_MODULE_CONTROLLER_H_
#define _MAIN_MODULE_CONTROLLER_H_

//UI include
#include "ui_MainModule.h"

//Qt includes
#include <QtCore/QSignalMapper>

//Dependency includes
#include "glm/vec3.hpp"

//Project includes
#include "DutagaciDialog.h"
#include "GLCanvas.h"
#include "Measure.h"
#include "ModuleController.h"
#include "SceneInformationBuilder.h"
#include "SphereOfViewpoints.h"

#include <filesystem>

namespace Ui {
    class MainModule;
}

/// Controller for the view of the main module
class MainModuleController : public ModuleController
{
    Q_OBJECT
public:
    explicit MainModuleController(QWidget *pParent = 0);
    ~MainModuleController();

    void CreateModuleMenus();
    void ActiveModule();

protected:
    void keyPressEvent(QKeyEvent *pEvent);
    void mouseMoveEvent(QMouseEvent *pEvent);
    void wheelEvent(QWheelEvent *pEvent);
    void mousePressEvent(QMouseEvent *pEvent);

private:
    /// Scene related methods
    void LoadScene(const std::filesystem::path &pFileName);

    /// Mesh of viewpoints related methods
    void LoadViewpoints(int pWidthResolution, bool pFaceCulling);
    void LoadViewpointsFromSphere(float pRadius, float pAngle, float pAspectRatio, unsigned char pSubdivision, int pWidthResolution, bool pFaceCulling);
    void ChangeNumberOfViewpoints(size_t pNumberOfViewpoints);
    void SaveViewpointMeasuresInformation(const QString &pFileName);
    size_t NextViewpoint();
    void SetViewpoint(size_t pViewpoint);
    void ShowViewpointInformation(size_t pViewpoint);
    void LoadDutagaciViewpoints();

    /// Altres
    void UpdateRenderingGUI();
    QString GetScreenshotName(size_t pViewpoint);

    QMenu* mMenuVisualization;
    QAction* mActionExport;
    QAction* mActionViewpointsSphere;

    DutagaciDialog* mDutagaciDialog;
    std::vector<glm::vec3> mDutagaciViewpoints;

    Ui::MainModule* mUi;

    GLCanvas *mOpenGLCanvas;
    std::shared_ptr<Scene> mScene;
    std::shared_ptr<SphereOfViewpoints> mSphereOfViewpoints;
    SceneInformationBuilder *mSceneInformationBuilder;

    bool mUpdateView;

    size_t mCurrentViewpoint;

    std::vector<Measure*> mViewpointMeasures;
    std::vector<QSlider*> mViewpointMeasuresSliders;

    QPoint mLastMousePosition;

private slots:
    //Menu
    void OpenModel();
    void ExportInformation();
    void WillDrawViewpointsSphere(bool pDraw);
    void RunDutagaciBenchmark();

    //Right panel
    void on_measureInViewpointSphereList_currentIndexChanged(int pValue);
    void on_alphaSlider_valueChanged(int pValue);
    void on_alphaSpinBox_valueChanged(double pValue);

    void SliderChanged(int pMeasure, int pValue);

    void on_loadViewpointsSphereButton_clicked();

    void on_computeCameraDistanceButton_clicked();
    void on_computeCameraAngleButton_clicked();
    void on_applyMaterialsCheckBox_clicked(bool pChecked);
    void on_backfaceCullingCheckBox_clicked(bool pChecked);

    /// Lights
    void on_applyIlluminationCheckBox_clicked(bool pChecked);
    void on_light1GroupBox_toggled(bool pChecked);
    void on_light1ColorButton_clicked();
    void on_light2GroupBox_toggled(bool pChecked);
    void on_light2ColorButton_clicked();
    void on_ambientLightAmountSpinBox_valueChanged(double pValue);

    /// Screenshots
    void on_bestAndWorstViewsButton_clicked();
};

#endif
