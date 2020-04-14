/// \file GLCanvas.h
/// \class GLCanvas
/// \author Xavier Bonaventura
/// \author Copyright: (c) Universitat de Girona

#ifndef GL_CANVAS_H
#define GL_CANVAS_H

//GLEW has to be included before any OpenGL include
#include "glew.h"

#include "Camera.h"
#include "Geometry.h"
#include "GLSLProgram.h"
#include "GPUScene.h"
#include "LightSettings.h"
#include "PerspectiveCamera.h"
#include "Scene.h"

#include <QtOpenGL/QGLWidget>

#include "glm/vec3.hpp"

#include <memory>
#include <vector>

/// Class to do the OpenGL render
class GLCanvas : public QGLWidget
{
    Q_OBJECT

public:
    /// Constructor
    explicit GLCanvas(QWidget *pParent = nullptr);
    /// Destructor
    ~GLCanvas();

    /// Load an scene
    void LoadScene(std::shared_ptr<Scene> pScene , const Camera* pCamera = nullptr);
    /// Initialize the list of meshes that will be render with the color per vertex with the parameter
    void SetPerVertexMesh(Geometry* pPerVertexMesh);
    /// Add a mesh that will be render with the color per vertex
    void AddPerVertexMesh(Geometry* pPerVertexMesh);

    /// Save a screenshot of the renderer
    QString SaveScreenshot( const QString &pFileName );

    void PanActiveCamera(const glm::vec2& pStartPoint, const glm::vec2& pEndPoint);
    void RotateActiveCamera(const glm::vec2& pStartPoint, const glm::vec2& pEndPoint);
    void MoveActiveCamera(float pDeltaFactor);
    void ResetActiveCamera();

    /// Set the camera used to render
    void SetCamera(std::unique_ptr<Camera> pCamera);
    /// Get the scene rendered
    std::shared_ptr<Scene> GetScene();

    void ConfigureFirstLight(const LightSettings& settings);
    const LightSettings& GetFirstLightConfiguration() const;
    void ConfigureSecondLight(const LightSettings& settings);
    const LightSettings& GetSecondLightConfiguration() const;
    void ApplyIllumination(bool enabled);
    void ApplyFaceCulling(bool enabled);
    void SetAmbientLightIntensity(float intensity);

public slots:
    /// Set if the bounding boxes will be drawn
    void WillDrawBoundingBox(bool pDraw);
    /// Set if the bounding spheres will be drawn
    void WillDrawBoundingSphere(bool pDraw);
    /// Set if the materials will be applied for the rendering
    void ApplyMaterials(bool pApplyMaterials);

protected:
    /// Initialize the OpenGL
    void initializeGL();
    /// OpenGL paint method
    void paintGL();
    /// Method to catch the resize event
    void resizeGL(int pWidth, int pHeight);
    /// Method to catch the key events
    void keyPressEvent(QKeyEvent *pEvent);

private:
    void DrawGeometryBoundingVolumes();

    void RecomputeViewport();

    void LoadShaders();

    /// Initialize the dual depth peeling render targets
    void InitDualPeelingRenderTargets();
    /// Delete the dual depth peeling render targets
    void DeleteDualPeelingRenderTargets();

    bool mOpenGLInitialized = false;
    bool mDrawBoundingBox = false;
    bool mDrawBoundingSphere = false;
    bool mDrawViewpointSphereInWireframe = false;
    /// Boolean to know if the materials will be applied for the rendering
    bool mApplyMaterials = true;

    LightSettings mFirstLightSettings = {false, glm::vec3(), glm::vec3()};
    LightSettings mSecondLightSettings = {false, glm::vec3(), glm::vec3()};
    bool mApplyIllumination = true;
    bool mApplyFaceCulling = true;
    float mAmbientLightIntensity = 1.0f;

    /// Camera used for the rendering
    std::unique_ptr<Camera> mFreeCamera{nullptr};

    /// Scene
    std::shared_ptr<Scene> mScene = nullptr;
    /// Scene that will be used for the rendering
    std::unique_ptr<GPUScene> mGPUScene = nullptr;
    /// Meshes that will be rendered by per vertex color
    std::vector<Geometry*> mPerVertexColorMeshes;

    /// Shader used to initialize the min-max depth buffer for the dual depth peeling
    std::unique_ptr<GLSLProgram> mShaderDualInit = nullptr;
    /// Shader used to do the main pass of the renderer
    std::unique_ptr<GLSLProgram> mShaderDualPeel = nullptr;
    /// Shader used to do the main pass of the renderer for the gizmos
    std::unique_ptr<GLSLProgram> mShaderDualPeelPerVertexColor = nullptr;
    /// Shader used to alpha-blend the back color for the dual depth peeling
    std::unique_ptr<GLSLProgram> mShaderDualBlend = nullptr;
    /// Shader used to combinte the color of the front and the back buffer for the dual depth peeling
    std::unique_ptr<GLSLProgram> mShaderDualFinal = nullptr;

    /// Variable used for the dual deep peeling
    GLuint mQueryId;
    /// Variable used for the dual deep peeling
    GLuint mDualBackBlenderFboId;
    /// Variable used for the dual deep peeling
    GLuint mDualPeelingSingleFboId;
    /// Variable used for the dual deep peeling
    GLuint mDualDepthTexId[2];
    /// Variable used for the dual deep peeling
    GLuint mDualFrontBlenderTexId[2];
    /// Variable used for the dual deep peeling
    GLuint mDualBackTempTexId[2];
    /// Variable used for the dual deep peeling
    GLuint mDualBackBlenderTexId;
    /// Variable used for the dual deep peeling
    const GLenum mDrawBuffers[7] = {GL_COLOR_ATTACHMENT0,
                                    GL_COLOR_ATTACHMENT1,
                                    GL_COLOR_ATTACHMENT2,
                                    GL_COLOR_ATTACHMENT3,
                                    GL_COLOR_ATTACHMENT4,
                                    GL_COLOR_ATTACHMENT5,
                                    GL_COLOR_ATTACHMENT6};

    /// Background color
    const glm::vec3 mBackgroundColor = glm::vec3(0.5f, 0.5f, 0.5f);

    /// Full screen quad mesh used for the rendering
    std::unique_ptr<Geometry> mMeshFullScreenQuad;

    /// Width of the canvas
    int mWinWidth = 0;
    /// Height of the canvas
    int mWinHeight = 0;
    /// Area of the canvas that will be used for the rendering
    QRect mViewport;
};

#endif
