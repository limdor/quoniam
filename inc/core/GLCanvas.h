/// \file GLCanvas.h
/// \class GLCanvas
/// \author Xavier Bonaventura
/// \author Copyright: (c) Universitat de Girona

#ifndef _GL_CANVAS_H_
#define _GL_CANVAS_H_

//GLEW has to be included before any OpenGL include
#include "glew.h"

//Qt includes
#include <QGLWidget>
#include <QVector>

//Dependency includes
#include "glm/vec3.hpp"

//Project includes
#include "Camera.h"
#include "Geometry.h"
#include "GLSLProgram.h"
#include "GPUScene.h"
#include "PerspectiveCamera.h"
#include "Scene.h"

/// Class to do the OpenGL render
class GLCanvas : public QGLWidget
{
    Q_OBJECT

public:
    /// Constructor
    explicit GLCanvas(QWidget *pParent = 0);
    /// Destructor
    ~GLCanvas();

    /// Load an scene
    void LoadScene(Scene* pScene , const Camera* pCamera = nullptr);
    /// Initialize the list of meshes that will be render with the color per vertex with the parameter
    void SetPerVertexMesh(Geometry* pPerVertexMesh);
    /// Add a mesh that will be render with the color per vertex
    void AddPerVertexMesh(Geometry* pPerVertexMesh);

    /// Get the program used to do the rendering
    GLSLProgram* GetShaderProgram() const;
    /// Save a screenshot of the renderer
    QString SaveScreenshot( const QString &pFileName );

    /// Set the camera used to render
    void SetCamera(const Camera* pCamera);
    /// Get the camera used to render
    Camera *GetCamera();
    /// Get the scene rendered
    Scene* GetScene();

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
    /// Draw the bounding volumes
    void DrawGeometryBoundingVolumes();

    /// Recompute the viewport
    void RecomputeViewport();

    /// Load the shaders
    void LoadShaders();
    /// Delete the shaders
    void DeleteShaders();

    /// Initialize the dual depth peeling render targets
    void InitDualPeelingRenderTargets();
    /// Delete the dual depth peeling render targets
    void DeleteDualPeelingRenderTargets();

    /// Boolean to know if the bounding boxes will be drawn
    bool mDrawBoundingBox;
    /// Boolean to know if the bounding spheres will be drawn
    bool mDrawBoundingSphere;
    /// Boolean to know if the sphere of viewpoints will be drawn in wireframe mode
    bool mDrawWireframe;
    /// Boolean to know if the materials will be applied for the rendering
    bool mApplyMaterials;

    /// Camera used for the rendering
    Camera* mFreeCamera;

    /// Scene
    Scene* mScene;
    /// Scene that will be used for the rendering
    GPUScene* mGPUScene;
    /// Meshes that will be rendered by per vertex color
    QVector<Geometry*> mPerVertexColorMeshes;

    /// Shader used to initialize the min-max depth buffer for the dual depth peeling
    GLSLProgram *mShaderDualInit;
    /// Shader used to do the main pass of the renderer
    GLSLProgram *mShaderDualPeel;
    /// Shader used to do the main pass of the renderer for the gizmos
    GLSLProgram *mShaderDualPeelPerVertexColor;
    /// Shader used to alpha-blend the back color for the dual depth peeling
    GLSLProgram *mShaderDualBlend;
    /// Shader used to combinte the color of the front and the back buffer for the dual depth peeling
    GLSLProgram *mShaderDualFinal;

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
    GLenum mDrawBuffers[7];

    /// Background color
    glm::vec3 mBackgroundColor;

    /// Full screen quad mesh used for the rendering
    Geometry* mMeshFullScreenQuad;

    /// Width of the canvas
    int mWinWidth;
    /// Height of the canvas
    int mWinHeight;
    /// Area of the canvas that will be used for the rendering
    QRect mViewport;
};

#endif
