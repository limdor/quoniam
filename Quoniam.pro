QT += opengl #core and gui are already included

TARGET = Quoniam
TEMPLATE = app

# Application version
VERSION = 1.0.1

# Define the preprocessor macro to get the application version in our application
DEFINES += APP_VERSION=\\\"$$VERSION\\\"

CONFIG(debug, debug|release){
    COMPILATION = debug
}
else {
    COMPILATION = release
}

include(dependencies/assimp/assimp_dependency.pri)
include(dependencies/opencv/opencv_dependency.pri)
include(dependencies/qt/qt_installation.pri)
include(dependencies/glm/glm_dependency.pri)
include(dependencies/miniball/miniball_dependency.pri)
include(dependencies/glew/glew_dependency.pri)

win32 {
    RC_FILE = Quoniam.rc

    DESTDIR = $$OUT_PWD/bin/

    LIBS += opengl32.lib
    LIBS += glu32.lib

    equals(COMPILATION,debug){
        include(dependencies/vld/vld_dependency.pri)
    }
}

INCLUDEPATH +=\
    $$PWD/src \
    $$PWD/src/core \
    $$PWD/src/viewpoint-measures

OBJECTS_DIR = $$OUT_PWD/tmp/$$COMPILATION
RCC_DIR = $$OUT_PWD/tmp/$$COMPILATION

MOC_DIR = $$OUT_PWD/tmp
UI_DIR = $$OUT_PWD/tmp

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
    src/core/AxisAlignedBoundingBox.h \
    src/core/BoundingSphere.h \
    src/core/Camera.h \
    src/core/CameraController.h \
    src/core/Debug.h \
    src/core/Geometry.h \
    src/core/Gizmo.h \
    src/core/GLCanvas.h \
    src/core/GLSLProgram.h \
    src/core/GLSLShader.h \
    src/core/GPUGeometry.h \
    src/core/GPUScene.h \
    src/core/GPUSceneNode.h \
    src/core/LightSettings.h \
    src/core/Material.h \
    src/core/Mesh.h \
    src/core/OrthographicCamera.h \
    src/core/PerspectiveCamera.h \
    src/core/Scene.h \
    src/core/SceneLoader.h \
    src/core/SceneNode.h \
    src/core/SerializedSceneGeometry.h \
    src/core/Texture.h \
    src/core/TrackballCamera.h \
    src/viewpoint-measures/CurvatureEntropy.h \
    src/viewpoint-measures/DepthBasedVisualStability.h \
    src/viewpoint-measures/DepthDistribution.h \
    src/viewpoint-measures/FeixasSaliency.h \
    src/viewpoint-measures/HeuristicMeasure.h \
    src/viewpoint-measures/I2.h \
    src/viewpoint-measures/I3.h \
    src/viewpoint-measures/KullbackLeibler.h \
    src/viewpoint-measures/MaximumDepth.h \
    src/viewpoint-measures/NumberOfVisibleTriangles.h \
    src/viewpoint-measures/ProjectedArea.h \
    src/viewpoint-measures/SaliencyEVMI.h \
    src/viewpoint-measures/SilhouetteCurvature.h \
    src/viewpoint-measures/SilhouetteCurvatureExtrema.h \
    src/viewpoint-measures/SilhouetteEntropy.h \
    src/viewpoint-measures/SilhouetteLength.h \
    src/viewpoint-measures/StoevStrasser.h \
    src/viewpoint-measures/Unstability.h \
    src/viewpoint-measures/ViewpointEntropy.h \
    src/viewpoint-measures/VisibilityRatio.h \
    src/viewpoint-measures/VMI.h \
    src/DutagaciDialog.h \
    src/MainModuleController.h \
    src/MainWindow.h \
    src/Measure.h \
    src/ModuleController.h \
    src/ModuleTabWidget.h \
    src/ProjectedAreasMatrix.h \
    src/SceneInformationBuilder.h \
    src/SphereOfViewpoints.h \
    src/SpherePointCloud.h \
    src/Tools.h \
    src/ViewpointMeasureSlider.h

FORMS    += \
    src/DutagaciView.ui \
    src/MainModule.ui \
    src/MainWindow.ui

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
