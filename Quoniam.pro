#-------------------------------------------------
#
# Project created by QtCreator 2011-02-09T17:08:35
#
#-------------------------------------------------

QT += opengl #core and gui are already included

TARGET = Quoniam
TEMPLATE = app

# Application version
VERSION = 1.0.0

# Define the preprocessor macro to get the application version in our application
DEFINES += APP_VERSION=\\\"$$VERSION\\\"

CONFIG(debug, debug|release){
    COMPILATION = debug
}
else {
    COMPILATION = release
}

win32 {
    RC_FILE = Quoniam.rc

    DESTDIR = ./bin/win/$$COMPILATION

    LIBS += -L$$PWD/dependencies/lib/$$COMPILATION/ -lassimp
    LIBS += -L$$PWD/dependencies/lib/ -lglew32

    PWD_WIN = $$replace(PWD,"/","\\")
    # Copy supporting library DLLs
    QMAKE_POST_LINK += $$quote(copy "$$PWD_WIN\\dependencies\\bin\\win\\glew32.dll" "$$PWD_WIN\\bin\\win\\$$COMPILATION\\" $$escape_expand(\\n))
    equals(COMPILATION,debug){
        LIBS += -L$$PWD/dependencies/lib/$$COMPILATION/ -lopencv_core249d
        LIBS += -L$$PWD/dependencies/lib/$$COMPILATION/ -lopencv_imgproc249d
        LIBS += -L$$PWD/dependencies/lib/$$COMPILATION/ -lopencv_highgui249d

        QMAKE_POST_LINK += $$quote(copy "$$PWD_WIN\\dependencies\\bin\\win\\$$COMPILATION\\opencv_core249d.dll" "$$PWD_WIN\\bin\\win\\$$COMPILATION\\" $$escape_expand(\\n))
        QMAKE_POST_LINK += $$quote(copy "$$PWD_WIN\\dependencies\\bin\\win\\$$COMPILATION\\opencv_imgproc249d.dll" "$$PWD_WIN\\bin\\win\\$$COMPILATION\\" $$escape_expand(\\n))
        QMAKE_POST_LINK += $$quote(copy "$$PWD_WIN\\dependencies\\bin\\win\\$$COMPILATION\\opencv_highgui249d.dll" "$$PWD_WIN\\bin\\win\\$$COMPILATION\\" $$escape_expand(\\n))

        QT_INSTALL_BINS_WIN = $$[QT_INSTALL_BINS]
        QT_INSTALL_BINS_WIN = $$replace(QT_INSTALL_BINS_WIN,"/","\\")
        QMAKE_POST_LINK += $$quote(copy "$$QT_INSTALL_BINS_WIN\\Qt5Cored.dll" "$$PWD_WIN\\bin\\win\\$$COMPILATION\\" $$escape_expand(\\n))
        QMAKE_POST_LINK += $$quote(copy "$$QT_INSTALL_BINS_WIN\\Qt5Guid.dll" "$$PWD_WIN\\bin\\win\\$$COMPILATION\\" $$escape_expand(\\n))
        QMAKE_POST_LINK += $$quote(copy "$$QT_INSTALL_BINS_WIN\\Qt5OpenGLd.dll" "$$PWD_WIN\\bin\\win\\$$COMPILATION\\" $$escape_expand(\\n))
        QMAKE_POST_LINK += $$quote(copy "$$QT_INSTALL_BINS_WIN\\Qt5Widgetsd.dll" "$$PWD_WIN\\bin\\win\\$$COMPILATION\\" $$escape_expand(\\n))

        QMAKE_POST_LINK += $$quote(copy "$$PWD_WIN\\dependencies\\bin\\win\\release\\Assimp64.dll" "$$PWD_WIN\\bin\\win\\$$COMPILATION\\Assimp64d.dll" $$escape_expand(\\n))
    }
    else {
        LIBS += -L$$PWD/dependencies/lib/$$COMPILATION/ -lopencv_core249
        LIBS += -L$$PWD/dependencies/lib/$$COMPILATION/ -lopencv_imgproc249
        LIBS += -L$$PWD/dependencies/lib/$$COMPILATION/ -lopencv_highgui249

        QMAKE_POST_LINK += $$quote(copy "$$PWD_WIN\\dependencies\\bin\\win\\$$COMPILATION\\opencv_core249.dll" "$$PWD_WIN\\bin\\win\\$$COMPILATION\\" $$escape_expand(\\n))
        QMAKE_POST_LINK += $$quote(copy "$$PWD_WIN\\dependencies\\bin\\win\\$$COMPILATION\\opencv_imgproc249.dll" "$$PWD_WIN\\bin\\win\\$$COMPILATION\\" $$escape_expand(\\n))
        QMAKE_POST_LINK += $$quote(copy "$$PWD_WIN\\dependencies\\bin\\win\\$$COMPILATION\\opencv_highgui249.dll" "$$PWD_WIN\\bin\\win\\$$COMPILATION\\" $$escape_expand(\\n))

        QT_INSTALL_BINS_WIN = $$[QT_INSTALL_BINS]
        QT_INSTALL_BINS_WIN = $$replace(QT_INSTALL_BINS_WIN,"/","\\")
        QMAKE_POST_LINK += $$quote(copy "$$QT_INSTALL_BINS_WIN\\Qt5Core.dll" "$$PWD_WIN\\bin\\win\\$$COMPILATION\\" $$escape_expand(\\n))
        QMAKE_POST_LINK += $$quote(copy "$$QT_INSTALL_BINS_WIN\\Qt5Gui.dll" "$$PWD_WIN\\bin\\win\\$$COMPILATION\\" $$escape_expand(\\n))
        QMAKE_POST_LINK += $$quote(copy "$$QT_INSTALL_BINS_WIN\\Qt5OpenGL.dll" "$$PWD_WIN\\bin\\win\\$$COMPILATION\\" $$escape_expand(\\n))
        QMAKE_POST_LINK += $$quote(copy "$$QT_INSTALL_BINS_WIN\\Qt5Widgets.dll" "$$PWD_WIN\\bin\\win\\$$COMPILATION\\" $$escape_expand(\\n))

        QMAKE_POST_LINK += $$quote(copy "$$PWD_WIN\\dependencies\\bin\\win\\$$COMPILATION\\Assimp64.dll" "$$PWD_WIN\\bin\\win\\$$COMPILATION\\" $$escape_expand(\\n))
    }
}

INCLUDEPATH +=\
    $$PWD/dependencies/include \
    $$PWD/inc \
    $$PWD/inc/core \
    $$PWD/inc/viewpoint-measures

DEPENDPATH +=\
    $$PWD/dependencies/include \
    $$PWD/inc \
    $$PWD/inc/core \
    $$PWD/inc/viewpoint-measures

OBJECTS_DIR = ./tmp/$$COMPILATION
RCC_DIR = ./tmp/$$COMPILATION

MOC_DIR = ./tmp
UI_DIR = ./tmp

SOURCES +=\
    src/core/AxisAlignedBoundingBox.cpp \
    src/core/BoundingSphere.cpp \
    src/core/Camera.cpp \
    src/core/CameraController.cpp \
    src/core/Debug.cpp \
    src/core/Geometry.cpp \
    src/core/Gizmo.cpp \
    src/core/GLCanvas.cpp \
    src/core/GLSLProgram.cpp \
    src/core/GLSLShader.cpp \
    src/core/GPUGeometry.cpp \
    src/core/GPUScene.cpp \
    src/core/GPUSceneNode.cpp \
    src/core/Material.cpp \
    src/core/Mesh.cpp \
    src/core/OrthographicCamera.cpp \
    src/core/PerspectiveCamera.cpp \
    src/core/Scene.cpp \
    src/core/SceneLoader.cpp \
    src/core/SceneNode.cpp \
    src/core/SerializedSceneGeometry.cpp \
    src/core/Texture.cpp \
    src/core/TrackballCamera.cpp \
    src/viewpoint-measures/CurvatureEntropy.cpp \
    src/viewpoint-measures/DepthBasedVisualStability.cpp \
    src/viewpoint-measures/DepthDistribution.cpp \
    src/viewpoint-measures/FeixasSaliency.cpp \
    src/viewpoint-measures/HeuristicMeasure.cpp \
    src/viewpoint-measures/I2.cpp \
    src/viewpoint-measures/I3.cpp \
    src/viewpoint-measures/KullbackLeibler.cpp \
    src/viewpoint-measures/MaximumDepth.cpp \
    src/viewpoint-measures/NumberOfVisibleTriangles.cpp \
    src/viewpoint-measures/ProjectedArea.cpp \
    src/viewpoint-measures/SaliencyEVMI.cpp \
    src/viewpoint-measures/SilhouetteCurvature.cpp \
    src/viewpoint-measures/SilhouetteCurvatureExtrema.cpp \
    src/viewpoint-measures/SilhouetteEntropy.cpp \
    src/viewpoint-measures/SilhouetteLength.cpp \
    src/viewpoint-measures/StoevStrasser.cpp \
    src/viewpoint-measures/Unstability.cpp \
    src/viewpoint-measures/ViewpointEntropy.cpp \
    src/viewpoint-measures/VisibilityRatio.cpp \
    src/viewpoint-measures/VMI.cpp \
    src/DutagaciDialog.cpp \
    src/Main.cpp \
    src/MainModuleController.cpp \
    src/MainWindow.cpp \
    src/Measure.cpp \
    src/ModuleController.cpp \
    src/ModuleTabWidget.cpp \
    src/ProjectedAreasMatrix.cpp \
    src/SceneInformationBuilder.cpp \
    src/SphereOfViewpoints.cpp \
    src/SpherePointCloud.cpp \
    src/Tools.cpp \
    src/ViewpointMeasureSlider.cpp

HEADERS  += \
    inc/core/AxisAlignedBoundingBox.h \
    inc/core/BoundingSphere.h \
    inc/core/Camera.h \
    inc/core/CameraController.h \
    inc/core/Debug.h \
    inc/core/Geometry.h \
    inc/core/Gizmo.h \
    inc/core/GLCanvas.h \
    inc/core/GLSLProgram.h \
    inc/core/GLSLShader.h \
    inc/core/GPUGeometry.h \
    inc/core/GPUScene.h \
    inc/core/GPUSceneNode.h \
    inc/core/Material.h \
    inc/core/Mesh.h \
    inc/core/OrthographicCamera.h \
    inc/core/PerspectiveCamera.h \
    inc/core/Scene.h \
    inc/core/SceneLoader.h \
    inc/core/SceneNode.h \
    inc/core/SerializedSceneGeometry.h \
    inc/core/Texture.h \
    inc/core/TrackballCamera.h \
    inc/viewpoint-measures/CurvatureEntropy.h \
    inc/viewpoint-measures/DepthBasedVisualStability.h \
    inc/viewpoint-measures/DepthDistribution.h \
    inc/viewpoint-measures/FeixasSaliency.h \
    inc/viewpoint-measures/HeuristicMeasure.h \
    inc/viewpoint-measures/I2.h \
    inc/viewpoint-measures/I3.h \
    inc/viewpoint-measures/KullbackLeibler.h \
    inc/viewpoint-measures/MaximumDepth.h \
    inc/viewpoint-measures/NumberOfVisibleTriangles.h \
    inc/viewpoint-measures/ProjectedArea.h \
    inc/viewpoint-measures/SaliencyEVMI.h \
    inc/viewpoint-measures/SilhouetteCurvature.h \
    inc/viewpoint-measures/SilhouetteCurvatureExtrema.h \
    inc/viewpoint-measures/SilhouetteEntropy.h \
    inc/viewpoint-measures/SilhouetteLength.h \
    inc/viewpoint-measures/StoevStrasser.h \
    inc/viewpoint-measures/Unstability.h \
    inc/viewpoint-measures/ViewpointEntropy.h \
    inc/viewpoint-measures/VisibilityRatio.h \
    inc/viewpoint-measures/VMI.h \
    inc/DutagaciDialog.h \
    inc/MainModuleController.h \
    inc/MainWindow.h \
    inc/Measure.h \
    inc/ModuleController.h \
    inc/ModuleTabWidget.h \
    inc/ProjectedAreasMatrix.h \
    inc/SceneInformationBuilder.h \
    inc/SphereOfViewpoints.h \
    inc/SpherePointCloud.h \
    inc/Tools.h \
    inc/ViewpointMeasureSlider.h

FORMS    += \
    forms/DutagaciView.ui \
    forms/MainModule.ui \
    forms/MainWindow.ui

OTHER_FILES += \
    shaders/Basic.vert \
    shaders/ColorPerFace.frag \
    shaders/DualPeelingBlend.frag \
    shaders/DualPeelingFinal.frag \
    shaders/DualPeelingInit.frag \
    shaders/DualPeelingPeel.frag \
    shaders/DualPeelingPeel.vert \
    shaders/ShadeFragment.frag \
    shaders/ShadePerVertexColor.frag \
    shaders/ThermalScale.frag \
    documentation/Quoniam.vpp
