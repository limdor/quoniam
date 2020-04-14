//Definition include
#include "MainModuleController.h"

//Qt includes
#include <QtWidgets/QColorDialog>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QProgressDialog>
#include <QtCore/QTextStream>
#include <QtCore/QTime>
#include <QtCore/QXmlStreamWriter>

//Dependency includes
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"
#include "glm/geometric.hpp"
#include "glm/trigonometric.hpp"
#include "glm/exponential.hpp"
#include "glm/gtx/rotate_vector.hpp"

//Project includes
#include "CurvatureEntropy.h"
#include "Debug.h"
#include "DepthBasedVisualStability.h"
#include "DepthDistribution.h"
#include "FeixasSaliency.h"
#include "HeuristicMeasure.h"
#include "I2.h"
#include "I3.h"
#include "KullbackLeibler.h"
#include "LightSettings.h"
#include "MainWindow.h"
#include "MaximumDepth.h"
#include "NumberOfVisibleTriangles.h"
#include "StoevStrasser.h"
#include "OrthographicCamera.h"
#include "ProjectedArea.h"
#include "SaliencyEVMI.h"
#include "SceneInformationBuilder.h"
#include "SceneLoader.h"
#include "SilhouetteCurvature.h"
#include "SilhouetteCurvatureExtrema.h"
#include "SilhouetteEntropy.h"
#include "SilhouetteLength.h"
#include "Tools.h"
#include "Unstability.h"
#include "ViewpointEntropy.h"
#include "ViewpointMeasureSlider.h"
#include "VisibilityRatio.h"
#include "VMI.h"

MainModuleController::MainModuleController(QWidget *pParent): ModuleController(pParent),
    mUi(new Ui::MainModule), mDutagaciDialog(nullptr),
    mUpdateView(false),
    mScene(nullptr), mSphereOfViewpoints(nullptr),
    mCurrentViewpoint(0)
{
    mUi->setupUi(this);

    //Console configuration
    Debug::SetConsole(mUi->console);

    //GLCanvas initialization
    mOpenGLCanvas = mUi->Canvas;
    mOpenGLCanvas->show();

    QWidget* parent = mOpenGLCanvas;
    while(parent != nullptr)
    {
        parent->setMouseTracking(true);
        parent = parent->parentWidget();
    }
    CreateModuleMenus();

    //Shader uniforms initialization
    const QColor light1InitialColor{Qt::lightGray};
    const QColor light2InitialColor{Qt::lightGray};

    mUi->light1ColorLabel->setPalette(QPalette(light1InitialColor));
    mUi->light1ColorLabel->setAutoFillBackground(true);

    mUi->light2ColorLabel->setPalette(QPalette(light2InitialColor));
    mUi->light2ColorLabel->setAutoFillBackground(true);

    const LightSettings light_1_settings{
        mUi->light1GroupBox->isChecked(),
        glm::vec3(1.0f, 1.0f, 0.0f),
        glm::vec3(light1InitialColor.redF(), light1InitialColor.greenF(), light1InitialColor.blueF())
    };

    const LightSettings light_2_settings{
        mUi->light2GroupBox->isChecked(),
        glm::vec3(0.0f, -1.0f, 0.0f),
        glm::vec3(light2InitialColor.redF(), light2InitialColor.greenF(), light2InitialColor.blueF())
    };
    mOpenGLCanvas->ConfigureFirstLight(light_1_settings);
    mOpenGLCanvas->ConfigureSecondLight(light_2_settings);
    mOpenGLCanvas->ApplyIllumination(mUi->applyIlluminationCheckBox->isChecked());
    mOpenGLCanvas->ApplyFaceCulling(mUi->backfaceCullingCheckBox->isChecked());
    mOpenGLCanvas->SetAmbientLightIntensity(static_cast<float>(mUi->ambientLightAmountSpinBox->value()));

    mSceneInformationBuilder = new SceneInformationBuilder();

    mViewpointMeasures.push_back( new NumberOfVisibleTriangles("Number of visible triangles"));
    mViewpointMeasures.push_back( new ProjectedArea("Projected area") );
    mViewpointMeasures.push_back( new HeuristicMeasure("Heuristic") );
    mViewpointMeasures.push_back( new VisibilityRatio("Visibility ratio") );
    mViewpointMeasures.push_back( new KullbackLeibler("Kullback-Leibler") );
    mViewpointMeasures.push_back( new ViewpointEntropy("Viewpoint entropy") );
    mViewpointMeasures.push_back( new Unstability("Unstability") );
    //To use depth-based visual stability uncomment this line, it can take a lot of time to be computed
    //mViewpointMeasures.push_back( new DepthBasedVisualStability("Depth-based visual stability") );
    mViewpointMeasures.push_back( new VMI("VMI/I1") );
    mViewpointMeasures.push_back( new I2("I2") );
    mViewpointMeasures.push_back( new I3("I3") );
    mViewpointMeasures.push_back( new SaliencyEVMI("Saliency-based EVMI") );
    mViewpointMeasures.push_back( new FeixasSaliency("Saliency (Feixas definition)") );
    mViewpointMeasures.push_back( new CurvatureEntropy("Curvature entropy") );
    mViewpointMeasures.push_back( new SilhouetteCurvature("Silhouette curvature") );
    mViewpointMeasures.push_back( new SilhouetteCurvatureExtrema("Silhouette curvature extrema") );
    mViewpointMeasures.push_back( new SilhouetteEntropy("Silhouette entropy"));
    mViewpointMeasures.push_back( new SilhouetteLength("Silhouette length"));
    mViewpointMeasures.push_back( new MaximumDepth("Maximum depth"));
    mViewpointMeasures.push_back( new StoevStrasser("Stoev and Strasser") );
    mViewpointMeasures.push_back( new DepthDistribution("Depth distribution") );

    for( int i = 0; i < mViewpointMeasures.size(); i++ )
    {
        mUi->measureInViewpointSphereList->addItem( mViewpointMeasures.at(i)->GetName() );
    }

    QHBoxLayout* horizontalLayout = new QHBoxLayout();
    QLabel* minimumTextLabel = new QLabel("Minimum");
    QFont font = minimumTextLabel->font();
    font.setBold(true);
    minimumTextLabel->setFont(font);
    minimumTextLabel->setAlignment(Qt::AlignLeft);
    QLabel* maximumTextLabel = new QLabel("Maximum");
    maximumTextLabel->setFont(font);
    maximumTextLabel->setAlignment(Qt::AlignRight);
    horizontalLayout->addWidget(minimumTextLabel);
    horizontalLayout->addWidget(maximumTextLabel);
    QWidget* minMaxWidget = new QWidget();
    minMaxWidget->setLayout(horizontalLayout);
    mViewpointMeasuresSliders.resize( mViewpointMeasures.size() );
    QVBoxLayout* verticalLayout = new QVBoxLayout();
    verticalLayout->addWidget(minMaxWidget);
    for( int i = 0; i < mViewpointMeasures.size(); i++ )
    {
        QString text = mViewpointMeasures.at(i)->GetName();
        if(mViewpointMeasures.at(i)->IsMaximumBest())
        {
            text += "->";
        }
        else
        {
            text += "<-";
        }
        QLabel* textNameLabel = new QLabel(text);
        textNameLabel->setAlignment(Qt::AlignHCenter);
        verticalLayout->addWidget(textNameLabel);
        ViewpointMeasureSlider* slider = new ViewpointMeasureSlider(i, Qt::Horizontal);
        verticalLayout->addWidget(slider);
        mViewpointMeasuresSliders[i] = slider;
        connect(slider, SIGNAL(valueChanged(int,int)), this, SLOT(SliderChanged(int, int)));
    }

    verticalLayout->addStretch();
    QWidget* widget = new QWidget();
    widget->setLayout(verticalLayout);
    mUi->leftTabWidget->addTab(widget, "Mesures");

    //Add the possible amount of viewpoints in the combo box
    for( int i = 0; i < 4; i++ )
    {
        mUi->numberOfViewpoints->addItem( QString("%1").arg( glm::pow( 2.0f, 2.0f * mUi->numberOfViewpoints->count() ) * 10 + 2 ) );
    }
    mUi->numberOfViewpoints->setCurrentIndex(3);

    //Hide the part of the interfaces that we don't need
    mUi->leftTabWidget->hide();
    mUi->rightTabWidget->hide();

    on_computeCameraDistanceButton_clicked();

    UpdateRenderingGUI();
}

MainModuleController::~MainModuleController()
{
    for( int i = 0; i < mViewpointMeasures.size(); i++ )
    {
        delete mViewpointMeasures.at(i);
    }

    delete mSceneInformationBuilder;
    delete mOpenGLCanvas;
    delete mUi;
}

void MainModuleController::CreateModuleMenus()
{
    QMenu* menuFile = new QMenu("&File", this);

    QAction* actionOpen = new QAction("&Open...", this);
    actionOpen->setShortcut(Qt::CTRL + Qt::Key_O);
    menuFile->addAction(actionOpen);
    connect(actionOpen, SIGNAL(triggered()), this, SLOT(OpenModel()));

    mActionExport = new QAction("&Export...", this);
    mActionExport->setShortcut(Qt::CTRL + Qt::Key_E);
    mActionExport->setEnabled(false);
    menuFile->addAction(mActionExport);
    connect(mActionExport, SIGNAL(triggered()), this, SLOT(ExportInformation()));

    QAction* actionQuit = new QAction("&Quit", this);
    actionQuit->setShortcut(Qt::CTRL + Qt::Key_Q);
    menuFile->addAction(actionQuit);
    connect(actionQuit, SIGNAL(triggered()), this, SLOT(close()));

    mMenus.push_back(menuFile);

    mMenuVisualization = new QMenu("&Visualization", this);
    mMenuVisualization->setEnabled(false);

    QAction* actionBoundingBoxes = new QAction("Bounding &Boxes", this);
    actionBoundingBoxes->setCheckable(true);
    actionBoundingBoxes->setShortcut(Qt::CTRL + Qt::Key_B);
    mMenuVisualization->addAction(actionBoundingBoxes);
    connect(actionBoundingBoxes, SIGNAL(triggered(bool)), mOpenGLCanvas, SLOT(WillDrawBoundingBox(bool)));

    QAction* actionBoundingSpheres = new QAction("Bounding &Spheres", this);
    actionBoundingSpheres->setCheckable(true);
    actionBoundingSpheres->setShortcut(Qt::CTRL + Qt::Key_S);
    mMenuVisualization->addAction(actionBoundingSpheres);
    connect(actionBoundingSpheres, SIGNAL(triggered(bool)), mOpenGLCanvas, SLOT(WillDrawBoundingSphere(bool)));

    mActionViewpointsSphere = new QAction("&Viewpoints Sphere", this);
    mActionViewpointsSphere->setCheckable(true);
    mActionViewpointsSphere->setShortcut(Qt::CTRL + Qt::Key_V);
    mMenuVisualization->addAction(mActionViewpointsSphere);
    connect(mActionViewpointsSphere, SIGNAL(triggered(bool)), this, SLOT(WillDrawViewpointsSphere(bool)));

    mMenus.push_back(mMenuVisualization);

    QMenu* benchmarks = new QMenu("Benchmarks", this);
    QAction* actionRunDutagaci = new QAction("Run Dutagaci", this);
    benchmarks->addAction(actionRunDutagaci);
    connect(actionRunDutagaci, SIGNAL(triggered()), this, SLOT(RunDutagaciBenchmark()));

    mMenus.push_back(benchmarks);
}

void MainModuleController::ActiveModule()
{
    Debug::SetConsole(mUi->console);
}

void MainModuleController::keyPressEvent(QKeyEvent *pEvent)
{
    if( pEvent->key() == Qt::Key_N && mSphereOfViewpoints != nullptr )
    {
        int viewpoint = NextViewpoint();
        Debug::Log(QString("Viewpoint %1 selected").arg(mSphereOfViewpoints->GetViewpoint(viewpoint)->mName));

        mUpdateView = false;
        for( int i = 0; i < mViewpointMeasuresSliders.size(); i++ )
        {
            mViewpointMeasuresSliders.at(i)->setValue(mViewpointMeasures.at(i)->GetPosition(viewpoint));
        }
        mUpdateView = true;

        pEvent->accept();
    }
    else if( pEvent->key() == Qt::Key_I && mSphereOfViewpoints != nullptr )
    {
        ShowViewpointInformation(mCurrentViewpoint);
    }
    else if( pEvent->key() == Qt::Key_P )
    {
        QString fileName = QDateTime::currentDateTime().toString("yyyyMMdd-hhmmss");

        mOpenGLCanvas->SaveScreenshot( QString("Screenshot-%1.png").arg(fileName) );
    }
}

void MainModuleController::mouseMoveEvent(QMouseEvent *pEvent)
{
    const glm::vec2 startPoint{mLastMousePosition.x(), mLastMousePosition.y()};
    const glm::vec2 endPoint{pEvent->pos().x(), pEvent->pos().y()};
    if( pEvent->buttons() & Qt::RightButton )
    {
        mOpenGLCanvas->PanActiveCamera(startPoint, endPoint);
    }
    if( pEvent->buttons() & Qt::LeftButton )
    {
        mOpenGLCanvas->RotateActiveCamera(startPoint, endPoint);
    }
    mLastMousePosition = pEvent->pos();
}

void MainModuleController::wheelEvent(QWheelEvent *pEvent)
{
    if(  mOpenGLCanvas->hasFocus() )
    {
        int numDegrees = pEvent->delta() / 8;
        float deltaFactor = (numDegrees / 360.0f) * 2.0f;
        mOpenGLCanvas->MoveActiveCamera(deltaFactor);
    }
}

void MainModuleController::mousePressEvent(QMouseEvent *pEvent)
{
    mOpenGLCanvas->setFocus();
    mLastMousePosition = pEvent->pos();
}

void MainModuleController::LoadScene(const QString &pFileName)
{
    QTime t;

    QApplication::setOverrideCursor( Qt::WaitCursor );
    t.start();
    Debug::Log(QString("Loading %1").arg(pFileName));

    mUpdateView = true;
    mUi->leftTabWidget->hide();
    mActionExport->setEnabled(false);
    mActionViewpointsSphere->setEnabled(false);
    mUi->rightTabWidget->setTabEnabled(mUi->rightTabWidget->indexOf(mUi->othersTab), false);
    mUi->rightTabWidget->setTabEnabled(mUi->rightTabWidget->indexOf(mUi->screenshotsTab), false);
    mMenuVisualization->setEnabled(true);
    mUi->applyMaterialsCheckBox->setChecked(true);
    mUi->rightTabWidget->show();

    mSphereOfViewpoints = nullptr;
    mScene = SceneLoader::LoadScene(pFileName);
    mScene->ShowInformation();
    mOpenGLCanvas->LoadScene(mScene);
    Debug::Log( QString("MainWindow::LoadScene - Total time elapsed: %1 ms").arg( t.elapsed() ) );

    on_applyMaterialsCheckBox_clicked( mUi->applyMaterialsCheckBox->isChecked() );
    QApplication::restoreOverrideCursor();
}

void MainModuleController::LoadViewpoints(int pWidthResolution, bool pFaceCulling)
{
    QTime t;

    //Creation of the information channel
    mSceneInformationBuilder->CreateHistogram(mScene, mSphereOfViewpoints, pWidthResolution, pFaceCulling, true);

    mOpenGLCanvas->SetPerVertexMesh(mSphereOfViewpoints->GetMesh());
    mOpenGLCanvas->ApplyFaceCulling(pFaceCulling);
    mActionViewpointsSphere->setEnabled(true);

    QApplication::setOverrideCursor( Qt::WaitCursor );
    t.start();
    QProgressDialog progress(this);
    progress.setLabelText("Computing measures...");
    progress.setCancelButton(0);
    progress.setRange(0, mViewpointMeasures.size());
    progress.show();
    progress.setValue(0);
    qApp->processEvents();
    //Reinitialize the measures
    for( int i = 0; i < mViewpointMeasures.size(); i++ )
    {
        mViewpointMeasures.at(i)->SetComputed(false);
    }
    //Compute the viewpoint measures only if they have not being caluclated before through dependencies
    for( int i = 0; i < mViewpointMeasures.size(); i++ )
    {
        if(!mViewpointMeasures.at(i)->Computed())
        {
            progress.setLabelText(QString("Computing %1...").arg(mViewpointMeasures.at(i)->GetName()));
            mViewpointMeasures.at(i)->Compute(mSceneInformationBuilder);
        }
        progress.setValue(i + 1);
        qApp->processEvents();
    }
    progress.hide();
    Debug::Log( QString("MainWindow::Measures computed - Time elapsed: %1 ms").arg(t.elapsed()) );
    QApplication::restoreOverrideCursor();

    on_measureInViewpointSphereList_currentIndexChanged( mUi->measureInViewpointSphereList->currentIndex() );

    mUpdateView = false;
    ChangeNumberOfViewpoints( mSphereOfViewpoints->GetNumberOfViewpoints() );
    mUpdateView = true;
    SetViewpoint(0);
    mUi->leftTabWidget->show();
    mUi->rightTabWidget->setTabEnabled(mUi->rightTabWidget->indexOf(mUi->othersTab), true);
    mUi->rightTabWidget->setTabEnabled(mUi->rightTabWidget->indexOf(mUi->screenshotsTab), true);
    mUi->rightTabWidget->setCurrentIndex( mUi->rightTabWidget->indexOf(mUi->othersTab) );
    mMenuVisualization->setEnabled(true);
    mActionExport->setEnabled(true);
    mActionViewpointsSphere->setChecked(true);
}

void MainModuleController::LoadViewpointsFromSphere(float pRadius, float pAngle, float pAspectRatio, int pSubdivision, int pWidthResolution, bool pFaceCulling)
{
    float radius = mScene->GetBoundingSphere()->GetRadius();
    glm::vec3 center = mScene->GetBoundingSphere()->GetCenter();

    mSphereOfViewpoints = std::make_shared<SphereOfViewpoints>(pAngle, pAspectRatio);
    mSphereOfViewpoints->SetToQuasiUniform(pSubdivision);
    mSphereOfViewpoints->ApplyTransform(radius, radius * pRadius, center);

    LoadViewpoints(pWidthResolution, pFaceCulling);
}

void MainModuleController::ChangeNumberOfViewpoints(int pNumberOfViewpoints)
{
    //Modify the range of the sliders depending of the amount of viewpoints
    for( int i = 0; i < mViewpointMeasuresSliders.size(); i++ )
    {
        mViewpointMeasuresSliders.at(i)->setRange(0, pNumberOfViewpoints - 1);
    }
}

void MainModuleController::SaveViewpointMeasuresInformation(const QString &pFileName)
{
    QFile file(pFileName);

    QApplication::setOverrideCursor( Qt::WaitCursor );
    Debug::Log(QString("Exportant %1").arg(pFileName));

    if(file.open(QFile::WriteOnly))
    {
        QXmlStreamWriter stream(&file);
        stream.setAutoFormatting(true);
        stream.writeStartDocument();

        stream.writeStartElement("viewpoints");
        for( int i = 0; i < mSphereOfViewpoints->GetNumberOfViewpoints(); i++ )
        {
            stream.writeStartElement("viewpoint");
            stream.writeAttribute("name", mSphereOfViewpoints->GetViewpoint(i)->mName);
            for( int j = 0; j < mViewpointMeasures.size(); j++ )
            {
                QString name = mViewpointMeasures.at(j)->GetName();
                name = name.replace(" ","_");
                name = name.replace("(","_");
                name = name.replace(")","");
                name = name.replace("-","_");
                name = name.replace("|","_");
                stream.writeTextElement( QString("%1").arg(name), QString("%1").arg(mViewpointMeasures.at(j)->GetValue(i)) );
            }
            stream.writeEndElement();
        }
        stream.writeEndElement();
        stream.writeEndDocument();
        file.close();
        Debug::Log(QString("Information written to file: %1").arg(pFileName));
    }
    else
    {
        Debug::Error(QString("Impossible to write to file: %1").arg(pFileName));
    }
    QApplication::restoreOverrideCursor();
}

int MainModuleController::NextViewpoint()
{
    mCurrentViewpoint++;
    if( mCurrentViewpoint >= mSphereOfViewpoints->GetNumberOfViewpoints() )
        mCurrentViewpoint = 0;
    SetViewpoint(mCurrentViewpoint);
    return mCurrentViewpoint;
}

void MainModuleController::SetViewpoint(int pViewpoint)
{
    mCurrentViewpoint = pViewpoint;
    Camera* currentCamera = mSphereOfViewpoints->GetViewpoint(mCurrentViewpoint);
    mOpenGLCanvas->SetCamera( currentCamera->Clone() );

    //ShowViewpointInformation(pViewpoint);
    mOpenGLCanvas->updateGL();

    mUpdateView = false;
    for( int i = 0; i < mViewpointMeasuresSliders.size(); i++ )
    {
        mViewpointMeasuresSliders.at(i)->setValue(mViewpointMeasures.at(i)->GetPosition(pViewpoint));
    }
    mUpdateView = true;
}

void MainModuleController::ShowViewpointInformation(int pViewpoint)
{
    Debug::Log(QString("Selected viewpoint: %1").arg(pViewpoint));
    glm::vec3 position = mSphereOfViewpoints->GetViewpoint(pViewpoint)->GetPosition();
    Debug::Log(QString("Position: %1, %2, %3").arg(position.x).arg(position.y).arg(position.z));
    float radius = mScene->GetBoundingSphere()->GetRadius();
    glm::vec3 center = mScene->GetBoundingSphere()->GetCenter();
    position = (position - center) / (radius * 3.0f);
    Debug::Log(QString("Normalized position: %1, %2, %3").arg(position.x).arg(position.y).arg(position.z));
    //Debug::Log(QString("  Latitude: %1").arg(mGeographicViewpoints[pViewpoint].y));
    //Debug::Log(QString("  Longitude: %1").arg(mGeographicViewpoints[pViewpoint].z));
    Debug::Log(QString("Viewpoint quality measures:"));
    for( int i = 0; i < mViewpointMeasures.size(); i++ )
    {
        Debug::Log( QString("  %1: %2").arg(mViewpointMeasures.at(i)->GetName()).arg(mViewpointMeasures.at(i)->GetValue(pViewpoint)) );
    }
}

void MainModuleController::LoadDutagaciViewpoints()
{
    QFile file("geodesic_sphere.txt");

    if( file.open(QIODevice::ReadOnly | QIODevice::Text) )
    {
        QTextStream textReader(&file);

        while( !textReader.atEnd() )
        {
            QStringList list = textReader.readLine().split(QRegExp("\\s+"), QString::SkipEmptyParts);
            if( list.size() == 3 )
            {
                mDutagaciViewpoints.push_back( glm::vec3( list.at(0).toFloat(), list.at(1).toFloat(), list.at(2).toFloat() ) );
            }
            else
            {
                Debug::Error( "File format unexpected!" );
            }

        }
        file.close();
    }
    else
    {
        if(!file.exists())
        {
            Debug::Error(QString("File with viewpoints not found"));
        }
        else
        {
            Debug::Error(QString("Impossible to open file with viewpoints"));
        }
    }
}

void MainModuleController::UpdateRenderingGUI()
{
    if(mUi->applyIlluminationCheckBox->isChecked())
    {
        mUi->light1ColorLabel->setEnabled(true);
        mUi->light1ColorButton->setEnabled(true);
        mUi->light2ColorLabel->setEnabled(true);
        mUi->light2ColorButton->setEnabled(true);
    }
    else
    {
        mUi->light1ColorLabel->setEnabled(false);
        mUi->light1ColorButton->setEnabled(false);
        mUi->light2ColorLabel->setEnabled(false);
        mUi->light2ColorButton->setEnabled(false);
    }
}

QString MainModuleController::GetScreenshotName(int pViewpoint)
{
    QString name = mScene->GetName();
    name += "_";
    if( mUi->applyMaterialsCheckBox->isChecked() )
    {
        name += "Textured_";
    }
    if( pViewpoint != -1 )
    {
        name += mSphereOfViewpoints->GetViewpoint(mCurrentViewpoint)->mName;
    }
    name.replace( " ", "_" );
    name.replace( ".", "_" );

    return name;
}

void MainModuleController::OpenModel()
{
    const QString fileName = QFileDialog::getOpenFileName(this, tr("Choose a model"), "./", tr("Supported models (*.3ds *.dae *.off *.ply *.obj);;3ds Max 3DS (*.3ds);;Collada (*.dae);;Object File Format (*.off);;Stanford Polygon Library (*.ply);;Wavefront Object (*.obj);;All files (*.*)"));
    if(!fileName.isEmpty())
    {
        LoadScene(fileName);
    }
}

void MainModuleController::ExportInformation()
{
    const QString fileName = QFileDialog::getSaveFileName(this, tr("Choose a file to export"), "./", tr("XML file (*.xml);;All files (*.*)"));
    if(!fileName.isEmpty())
    {
        SaveViewpointMeasuresInformation(fileName);
    }
}

void MainModuleController::WillDrawViewpointsSphere(bool pDraw)
{
    if(mSphereOfViewpoints != nullptr)
    {
        //mSphereOfViewpoints->GetMesh()->SetVisible(pDraw);
        mOpenGLCanvas->updateGL();
    }
}

void MainModuleController::RunDutagaciBenchmark()
{
    if(mDutagaciDialog == nullptr)
    {
        mDutagaciDialog = new DutagaciDialog(this);
        LoadDutagaciViewpoints();
    }
    int dialogReturn = mDutagaciDialog->exec();
    if(dialogReturn == QDialog::Accepted)
    {
        QVector< QVector<glm::vec3> > bestViewpoints(mViewpointMeasures.size());
        QVector<QString> filestoLoad = mDutagaciDialog->GetFilesToLoad();
        float angle = mDutagaciDialog->GetAngle();
        float distance = mDutagaciDialog->GetDistance();
        int numberOfFiles = filestoLoad.size();
        for( int i = 0; i < numberOfFiles; i++ )
        {
            LoadScene(filestoLoad.at(i));
            LoadViewpointsFromSphere( distance, angle, 1.0f, 3, 640, false );
            float radius = mScene->GetBoundingSphere()->GetRadius();
            glm::vec3 center = mScene->GetBoundingSphere()->GetCenter();
            for( int j = 0; j < mViewpointMeasures.size(); j++ )
            {
                glm::vec3 position = mSphereOfViewpoints->GetViewpoint(mViewpointMeasures.at(j)->GetBestViewpoint())->GetPosition();
                position = (position - center) / (radius * 3.0f);
                bestViewpoints[j].push_back(position);
            }
        }
        for( int i = 0; i < mViewpointMeasures.size(); i++ )
        {
            QString fileName = QString{mViewpointMeasures.at(i)->GetName()}.replace("|","_").replace("/","_") + ".txt";
            QFile file(fileName);
            if( file.open(QFile::WriteOnly) )
            {
                QTextStream out(&file);
                for( int j = 0; j < numberOfFiles; j++ )
                {
                    out << bestViewpoints.at(i).at(j).x << " " << bestViewpoints.at(i).at(j).y << " " << bestViewpoints.at(i).at(j).z << "\n";
                }
                file.close();
                Debug::Log( QString("Information written to file: %1").arg(fileName) );
            }
            else
            {
                Debug::Error( QString("Impossible to write a file: %1").arg(fileName) );
            }
        }
    }
}

void MainModuleController::on_measureInViewpointSphereList_currentIndexChanged(int pValue)
{
    if( mSphereOfViewpoints != nullptr )
    {
        QVector<glm::vec4> colors;

        if( pValue < mViewpointMeasures.size() )
        {
            colors = Tools::ConvertFloatsToColors(mViewpointMeasures.at(pValue)->GetValues(), !mViewpointMeasures.at(pValue)->IsMaximumBest());
        }
        for( int i = 0; i < colors.size(); i++ )
        {
            colors[i].a = mUi->alphaSpinBox->value();
        }
        mSphereOfViewpoints->GetMesh()->SetColorData( colors.size(), colors.data() );
        mOpenGLCanvas->updateGL();
    }
}

void MainModuleController::on_alphaSlider_valueChanged(int pValue)
{
    mUi->alphaSpinBox->setValue( pValue / 100.0f );
    on_measureInViewpointSphereList_currentIndexChanged(mUi->measureInViewpointSphereList->currentIndex());
}

void MainModuleController::on_alphaSpinBox_valueChanged(double pValue)
{
    mUi->alphaSlider->setValue( (int)(pValue * 100) );
    on_measureInViewpointSphereList_currentIndexChanged(mUi->measureInViewpointSphereList->currentIndex());
}

void MainModuleController::SliderChanged(int pMeasure, int pValue)
{
    if(mUpdateView)
    {
        unsigned int viewpoint = mViewpointMeasures.at(pMeasure)->GetNth(pValue);
        SetViewpoint(viewpoint);
    }
}

void MainModuleController::on_loadViewpointsSphereButton_clicked()
{
    LoadViewpointsFromSphere( mUi->cameraDistanceSpinBox->value(), mUi->cameraAngleSpinBox->value(), mUi->cameraAspectRatioSpinBox->value(), mUi->numberOfViewpoints->currentIndex(), mUi->widthResolutionSpinBox->value(), mUi->faceCullingCheckBox->isChecked() );
}

void MainModuleController::on_computeCameraDistanceButton_clicked()
{
    mUi->cameraAngleSpinBox->setValue( 2.0f * glm::degrees( glm::asin( 1.0f / (float)mUi->cameraDistanceSpinBox->value() ) ) );
    mUi->cameraAspectRatioSpinBox->setValue( 1.0f );
}

void MainModuleController::on_computeCameraAngleButton_clicked()
{
    mUi->cameraDistanceSpinBox->setValue( 1.0f / glm::sin( glm::radians( mUi->cameraAngleSpinBox->value() / 2.0f ) ) );
    mUi->cameraAspectRatioSpinBox->setValue( 1.0f );
}

void MainModuleController::on_applyMaterialsCheckBox_clicked(bool pChecked)
{
    mOpenGLCanvas->ApplyMaterials(pChecked);
    UpdateRenderingGUI();
}

void MainModuleController::on_backfaceCullingCheckBox_clicked(bool pChecked)
{
    mOpenGLCanvas->ApplyFaceCulling(pChecked);
}

void MainModuleController::on_applyIlluminationCheckBox_clicked(bool pChecked)
{
    mOpenGLCanvas->ApplyIllumination(pChecked);
    UpdateRenderingGUI();
}

void MainModuleController::on_light1GroupBox_toggled(bool pChecked)
{
    LightSettings settings = mOpenGLCanvas->GetFirstLightConfiguration();
    settings.enabled = pChecked;
    mOpenGLCanvas->ConfigureFirstLight(settings);
}

void MainModuleController::on_light1ColorButton_clicked()
{
    QColor color = QColorDialog::getColor(mUi->light1ColorLabel->palette().color(QPalette::Background), this);
    if( color.isValid() )
    {
        mUi->light1ColorLabel->setPalette(QPalette(color));
        mUi->light1ColorLabel->setAutoFillBackground(true);
    }
    LightSettings settings = mOpenGLCanvas->GetFirstLightConfiguration();
    settings.color = glm::vec3(color.redF(), color.greenF(), color.blueF());
    mOpenGLCanvas->ConfigureFirstLight(settings);
}

void MainModuleController::on_light2GroupBox_toggled(bool pChecked)
{
    LightSettings settings = mOpenGLCanvas->GetSecondLightConfiguration();
    settings.enabled = pChecked;
    mOpenGLCanvas->ConfigureSecondLight(settings);
}

void MainModuleController::on_light2ColorButton_clicked()
{
    QColor color = QColorDialog::getColor(mUi->light2ColorLabel->palette().color(QPalette::Background), this);
    if( color.isValid() )
    {
        mUi->light2ColorLabel->setPalette(QPalette(color));
        mUi->light2ColorLabel->setAutoFillBackground(true);
    }
    LightSettings settings = mOpenGLCanvas->GetSecondLightConfiguration();
    settings.color = glm::vec3(color.redF(), color.greenF(), color.blueF());
    mOpenGLCanvas->ConfigureSecondLight(settings);
}

void MainModuleController::on_ambientLightAmountSpinBox_valueChanged(double pValue)
{
    mOpenGLCanvas->SetAmbientLightIntensity(static_cast<float>(pValue));
}

void MainModuleController::on_bestAndWorstViewsButton_clicked()
{
    QString sceneName = mScene->GetName();

    mUi->applyIlluminationCheckBox->setChecked(true);
    mOpenGLCanvas->ApplyIllumination(true);
    mUi->applyMaterialsCheckBox->setChecked(true);
    on_applyMaterialsCheckBox_clicked(true);
    mUi->alphaSpinBox->setValue( 0.01f );
    int numberOfMeasures = mViewpointMeasures.size();

    for( int measureIndex = 0; measureIndex < numberOfMeasures; measureIndex++ )
    {
        Measure* measure = mViewpointMeasures.at(measureIndex);
        QString measureName = measure->GetName();
        measureName = measureName.replace(" ","_");
        measureName = measureName.replace("(","_");
        measureName = measureName.replace(")","");
        measureName = measureName.replace("-","_");
        measureName = measureName.replace("|","_");
        measureName = measureName.replace("/","_");

        unsigned int viewpoint = measure->GetNth(0);
        SetViewpoint(viewpoint);
        if(measure->IsMaximumBest())
        {
            mOpenGLCanvas->SaveScreenshot( QString("%1_Worst_%2.png").arg(sceneName).arg(measureName) );
        }
        else
        {
            mOpenGLCanvas->SaveScreenshot( QString("%1_Best_%2.png").arg(sceneName).arg(measureName) );
        }

        mUi->alphaSpinBox->setValue( 1.0f );

        on_measureInViewpointSphereList_currentIndexChanged( measureIndex );

        mOpenGLCanvas->ResetActiveCamera();
        if(measure->IsMaximumBest())
        {
            mOpenGLCanvas->SaveScreenshot( QString("%1_Worst_%2_Sphere.png").arg(sceneName).arg(measureName) );
        }
        else
        {
            mOpenGLCanvas->SaveScreenshot( QString("%1_Best_%2_Sphere.png").arg(sceneName).arg(measureName) );
        }
        mOpenGLCanvas->ResetActiveCamera();

        mUi->alphaSpinBox->setValue( 0.01f );

        viewpoint = measure->GetNth( mSphereOfViewpoints->GetNumberOfViewpoints() - 1 );
        SetViewpoint(viewpoint);
        if(measure->IsMaximumBest())
        {
            mOpenGLCanvas->SaveScreenshot( QString("%1_Best_%2.png").arg(sceneName).arg(measureName) );
        }
        else
        {
            mOpenGLCanvas->SaveScreenshot( QString("%1_Worst_%2.png").arg(sceneName).arg(measureName) );
        }

        mUi->alphaSpinBox->setValue( 1.0f );

        on_measureInViewpointSphereList_currentIndexChanged( measureIndex );

        mOpenGLCanvas->ResetActiveCamera();
        if(measure->IsMaximumBest())
        {
            mOpenGLCanvas->SaveScreenshot( QString("%1_Best_%2_Sphere.png").arg(sceneName).arg(measureName) );
        }
        else
        {
            mOpenGLCanvas->SaveScreenshot( QString("%1_Worst_%2_Sphere.png").arg(sceneName).arg(measureName) );
        }
        mOpenGLCanvas->ResetActiveCamera();

        mUi->alphaSpinBox->setValue( 0.01f );
    }
}
