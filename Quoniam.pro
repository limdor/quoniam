#-------------------------------------------------
#
# Project created by QtCreator 2011-02-09T17:08:35
#
#-------------------------------------------------

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

include(assimp_dependency.pri)

win32 {
    RC_FILE = Quoniam.rc

    DESTDIR = $$OUT_PWD/bin/

    LIBS += -L$$PWD/dependencies/glew/lib/ -lglew32
    LIBS += opengl32.lib
    LIBS += glu32.lib

    PWD_WIN = $$replace(PWD,"/","\\")
    OUT_PWD_WIN = $$replace(OUT_PWD,"/","\\")
    # Copy supporting library DLLs
    QMAKE_POST_LINK += $$quote(copy "$$PWD_WIN\\dependencies\\glew\\bin\\glew32.dll" "$$OUT_PWD_WIN\\bin\\" $$escape_expand(\\n))
    equals(COMPILATION,debug){
        LIBS += -L$$PWD/dependencies/vld/lib/ -lvld
        LIBS += -L$$PWD/dependencies/opencv/lib/$$COMPILATION/ -lopencv_core331d
        LIBS += -L$$PWD/dependencies/opencv/lib/$$COMPILATION/ -lopencv_imgproc331d
        LIBS += -L$$PWD/dependencies/opencv/lib/$$COMPILATION/ -lopencv_imgcodecs331d
        LIBS += -L$$PWD/dependencies/opencv/lib/$$COMPILATION/ -lopencv_highgui331d

        QMAKE_POST_LINK += $$quote(copy "$$PWD_WIN\\dependencies\\opencv\\bin\\$$COMPILATION\\opencv_core331d.dll" "$$OUT_PWD_WIN\\bin\\" $$escape_expand(\\n))
        QMAKE_POST_LINK += $$quote(copy "$$PWD_WIN\\dependencies\\opencv\\bin\\$$COMPILATION\\opencv_imgproc331d.dll" "$$OUT_PWD_WIN\\bin\\" $$escape_expand(\\n))
        QMAKE_POST_LINK += $$quote(copy "$$PWD_WIN\\dependencies\\opencv\\bin\\$$COMPILATION\\opencv_imgcodecs331d.dll" "$$OUT_PWD_WIN\\bin\\" $$escape_expand(\\n))
        QMAKE_POST_LINK += $$quote(copy "$$PWD_WIN\\dependencies\\opencv\\bin\\$$COMPILATION\\opencv_highgui331d.dll" "$$OUT_PWD_WIN\\bin\\" $$escape_expand(\\n))

        QT_INSTALL_BINS_WIN = $$[QT_INSTALL_BINS]
        QT_INSTALL_BINS_WIN = $$replace(QT_INSTALL_BINS_WIN,"/","\\")
        QT_INSTALL_PLUGINS_WIN = $$[QT_INSTALL_PLUGINS]
        QT_INSTALL_PLUGINS_WIN = $$replace(QT_INSTALL_PLUGINS_WIN,"/","\\")
        QMAKE_POST_LINK += $$quote(copy "$$QT_INSTALL_BINS_WIN\\Qt5Cored.dll" "$$OUT_PWD_WIN\\bin\\" $$escape_expand(\\n))
        QMAKE_POST_LINK += $$quote(copy "$$QT_INSTALL_BINS_WIN\\Qt5Guid.dll" "$$OUT_PWD_WIN\\bin\\" $$escape_expand(\\n))
        QMAKE_POST_LINK += $$quote(copy "$$QT_INSTALL_BINS_WIN\\Qt5OpenGLd.dll" "$$OUT_PWD_WIN\\bin\\" $$escape_expand(\\n))
        QMAKE_POST_LINK += $$quote(copy "$$QT_INSTALL_BINS_WIN\\Qt5Widgetsd.dll" "$$OUT_PWD_WIN\\bin\\" $$escape_expand(\\n))
        QMAKE_POST_LINK += $$quote(xcopy "$$QT_INSTALL_PLUGINS_WIN\\platforms\\qwindowsd.dll" "$$OUT_PWD_WIN\\bin\\platforms\\" /Y $$escape_expand(\\n))

        QMAKE_POST_LINK += $$quote(copy "$$PWD_WIN\\dependencies\\vld\\bin\\vld_x64.dll" "$$OUT_PWD_WIN\\bin\\" $$escape_expand(\\n))
        QMAKE_POST_LINK += $$quote(copy "$$PWD_WIN\\dependencies\\vld\\bin\\dbghelp.dll" "$$OUT_PWD_WIN\\bin\\" $$escape_expand(\\n))
        QMAKE_POST_LINK += $$quote(copy "$$PWD_WIN\\dependencies\\vld\\bin\\Microsoft.DTfW.DHL.manifest" "$$OUT_PWD_WIN\\bin\\" $$escape_expand(\\n))
    }
    else {
        LIBS += -L$$PWD/dependencies/opencv/lib/$$COMPILATION/ -lopencv_core331
        LIBS += -L$$PWD/dependencies/opencv/lib/$$COMPILATION/ -lopencv_imgproc331
        LIBS += -L$$PWD/dependencies/opencv/lib/$$COMPILATION/ -lopencv_imgcodecs331
        LIBS += -L$$PWD/dependencies/opencv/lib/$$COMPILATION/ -lopencv_highgui331

        QMAKE_POST_LINK += $$quote(copy "$$PWD_WIN\\dependencies\\opencv\\bin\\$$COMPILATION\\opencv_core331.dll" "$$OUT_PWD_WIN\\bin\\" $$escape_expand(\\n))
        QMAKE_POST_LINK += $$quote(copy "$$PWD_WIN\\dependencies\\opencv\\bin\\$$COMPILATION\\opencv_imgproc331.dll" "$$OUT_PWD_WIN\\bin\\" $$escape_expand(\\n))
        QMAKE_POST_LINK += $$quote(copy "$$PWD_WIN\\dependencies\\opencv\\bin\\$$COMPILATION\\opencv_imgcodecs331.dll" "$$OUT_PWD_WIN\\bin\\" $$escape_expand(\\n))
        QMAKE_POST_LINK += $$quote(copy "$$PWD_WIN\\dependencies\\opencv\\bin\\$$COMPILATION\\opencv_highgui331.dll" "$$OUT_PWD_WIN\\bin\\" $$escape_expand(\\n))

        QT_INSTALL_BINS_WIN = $$[QT_INSTALL_BINS]
        QT_INSTALL_BINS_WIN = $$replace(QT_INSTALL_BINS_WIN,"/","\\")
        QT_INSTALL_PLUGINS_WIN = $$[QT_INSTALL_PLUGINS]
        QT_INSTALL_PLUGINS_WIN = $$replace(QT_INSTALL_PLUGINS_WIN,"/","\\")
        QMAKE_POST_LINK += $$quote(copy "$$QT_INSTALL_BINS_WIN\\Qt5Core.dll" "$$OUT_PWD_WIN\\bin\\" $$escape_expand(\\n))
        QMAKE_POST_LINK += $$quote(copy "$$QT_INSTALL_BINS_WIN\\Qt5Gui.dll" "$$OUT_PWD_WIN\\bin\\" $$escape_expand(\\n))
        QMAKE_POST_LINK += $$quote(copy "$$QT_INSTALL_BINS_WIN\\Qt5OpenGL.dll" "$$OUT_PWD_WIN\\bin\\" $$escape_expand(\\n))
        QMAKE_POST_LINK += $$quote(copy "$$QT_INSTALL_BINS_WIN\\Qt5Widgets.dll" "$$OUT_PWD_WIN\\bin\\" $$escape_expand(\\n))
        QMAKE_POST_LINK += $$quote(xcopy "$$QT_INSTALL_PLUGINS_WIN\\platforms\\qwindows.dll" "$$OUT_PWD_WIN\\bin\\platforms\\" /Y $$escape_expand(\\n))
    }
}

INCLUDEPATH +=\
    $$PWD/dependencies/ \
    $$PWD/dependencies/glew/include \
    $$PWD/dependencies/glm \
    $$PWD/dependencies/miniball \
    $$PWD/dependencies/opencv/include \
    $$PWD/dependencies/vld/include \
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
