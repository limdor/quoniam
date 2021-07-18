#include "GLCanvas.h"

#include "AxisAlignedBoundingBox.h"
#include "Debug.h"
#include "GPUGeometry.h"
#include "OrthographicCamera.h"
#include "Tools.h"
#include "TrackballCamera.h"

#include "glm/mat4x4.hpp"
#include "glm/vec2.hpp"
#include "glm/geometric.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/rotate_vector.hpp"

GLCanvas::GLCanvas(QWidget *pParent): QGLWidget(QGLFormat(QGL::SampleBuffers), pParent)
{
}

GLCanvas::~GLCanvas()
{
    //Delete framebuffers and textures needed for the dual depth peeling
    DeleteDualPeelingRenderTargets();

    glDeleteQueries(1, &mQueryId);
}

void GLCanvas::LoadScene(std::shared_ptr<Scene> pScene, const Camera* pCamera)
{
    makeCurrent();
    mScene = pScene;
    mGPUScene = std::make_unique<GPUScene>(pScene);
    mPerVertexColorMeshes.clear();

    if(pCamera != nullptr)
    {
        mFreeCamera = pCamera->Clone();
    }
    else
    {
        float radius = mScene->GetBoundingSphere()->GetRadius();
        glm::vec3 center = mScene->GetBoundingSphere()->GetCenter();

        glm::vec3 position = glm::vec3(center.x, center.y, center.z + radius * 2.0f);
        glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
        glm::vec3 front = center - position;
        glm::vec3 right = glm::cross( front, up );
        up = glm::cross( right, front );
        mFreeCamera = std::make_unique<PerspectiveCamera>(0.05f * radius, radius * 50.0f, center, up, position, 60.0f, mWinWidth/static_cast<float>(mWinHeight));
    }

    RecomputeViewport();
    updateGL();
}

void GLCanvas::SetPerVertexMesh(std::shared_ptr<Geometry> pPerVertexMesh)
{
    mPerVertexColorMeshes.clear();
    mPerVertexColorMeshes.push_back(pPerVertexMesh);
}

void GLCanvas::AddPerVertexMesh(std::shared_ptr<Geometry> pPerVertexMesh)
{
    mPerVertexColorMeshes.push_back(pPerVertexMesh);
}

QString GLCanvas::SaveScreenshot( const QString &pFileName )
{
    makeCurrent();
    const QString completePath = QString::fromStdString(Tools::GetProgramPath().string());
    const QImage image = grabFrameBuffer();
    if(!image.save( completePath + pFileName ))
    {
        Debug::Warning( "Screenshot have not been saved: " + (completePath + pFileName).toStdString());
    }
    return (completePath + pFileName);
}

void GLCanvas::PanActiveCamera(const glm::vec2& pStartPoint, const glm::vec2& pEndPoint)
{
    if( mFreeCamera != nullptr )
    {
        const glm::vec3 prevCamPosition = mFreeCamera->GetPosition();
        const glm::vec3 prevCamUpVector = glm::normalize( mFreeCamera->GetUp() );
        const glm::vec3 prevCamFrontVector = glm::normalize( mFreeCamera->GetLookAt() - prevCamPosition );

        const int glHalfWidth = width() / 2;
        const int glHalfHeight = height() / 2;
        glm::vec2 initVector(pStartPoint.x-glHalfWidth, pStartPoint.y-glHalfHeight);
        glm::vec2 finalVector(pEndPoint.x-glHalfWidth, pEndPoint.y-glHalfHeight);

        auto newCamera = mFreeCamera->Clone();
        newCamera->SetUp( glm::rotate(prevCamUpVector, glm::atan(initVector.y, initVector.x) - glm::atan(finalVector.y, finalVector.x), prevCamFrontVector ) );
        SetCamera(std::move(newCamera));
    }
}

void GLCanvas::RotateActiveCamera(const glm::vec2& pStartPoint, const glm::vec2& pEndPoint)
{
    if ( mFreeCamera != nullptr && mScene != nullptr )
    {
        auto boundingSphere = mScene->GetBoundingSphere();
        TrackballCamera::MoveCamera(pStartPoint, pEndPoint, *mFreeCamera, boundingSphere->GetCenter());
        updateGL();
    }
}

void GLCanvas::MoveActiveCamera(float pDeltaFactor)
{
    if( mFreeCamera != nullptr && mScene != nullptr )
    {
        auto boundingSphere = mScene->GetBoundingSphere();
        const glm::vec3 prevCamPosition = mFreeCamera->GetPosition();
        const glm::vec3 prevCamFrontVector = glm::normalize( mFreeCamera->GetLookAt() - prevCamPosition );

        float farPlane = mFreeCamera->GetFarPlane();
        farPlane -= boundingSphere->GetRadius() * pDeltaFactor;

        const glm::vec3 position = prevCamPosition + prevCamFrontVector * boundingSphere->GetRadius() * pDeltaFactor;
        mFreeCamera->SetFarPlane( farPlane );
        mFreeCamera->SetPosition( position );
        updateGL();
    }
}

void GLCanvas::ResetActiveCamera()
{
    if( mFreeCamera != nullptr && mScene != nullptr )
    {
        auto boundingSphere = mScene->GetBoundingSphere();
        const glm::vec3 prevCamPosition = mFreeCamera->GetPosition();
        const glm::vec3 prevCamFrontVector = glm::normalize( mFreeCamera->GetLookAt() - prevCamPosition );

        const glm::vec3 newCamPosition = prevCamPosition - prevCamFrontVector * boundingSphere->GetRadius() * 3.0f * 2.0f;
        mFreeCamera->SetPosition( newCamPosition );
        updateGL();
    }
}

void GLCanvas::SetCamera(std::unique_ptr<Camera> pCamera)
{
    makeCurrent();
    mFreeCamera = std::move(pCamera);
    RecomputeViewport();
    updateGL();
}

std::shared_ptr<Scene> GLCanvas::GetScene()
{
    return mScene;
}

void GLCanvas::ConfigureFirstLight(const LightSettings& settings)
{
    if(mOpenGLInitialized)
    {
        mShaderDualPeel->UseProgram();
        mShaderDualPeel->SetUniform("light1.enabled", settings.enabled);
        mShaderDualPeel->SetUniform("light1.lookAt", settings.look_at_vector);
        mShaderDualPeel->SetUniform("light1.irradiance", settings.color);
    }
    mFirstLightSettings = settings;
}

const LightSettings& GLCanvas::GetFirstLightConfiguration() const
{
    return mFirstLightSettings;
}

void GLCanvas::ConfigureSecondLight(const LightSettings& settings)
{
    if(mOpenGLInitialized)
    {
        mShaderDualPeel->UseProgram();
        mShaderDualPeel->SetUniform("light2.enabled", settings.enabled);
        mShaderDualPeel->SetUniform("light2.lookAt", settings.look_at_vector);
        mShaderDualPeel->SetUniform("light2.irradiance", settings.color);
    }
    mSecondLightSettings = settings;
}

const LightSettings& GLCanvas::GetSecondLightConfiguration() const
{
    return mSecondLightSettings;
}

void GLCanvas::ApplyIllumination(bool enabled)
{
    if(mOpenGLInitialized)
    {
        mShaderDualPeel->UseProgram();
        mShaderDualPeel->SetUniform("applyIllumination", enabled);
    }
    mApplyIllumination = enabled;
 }

void GLCanvas::ApplyFaceCulling(bool enabled)
{
    if(mOpenGLInitialized)
    {
        mShaderDualPeel->UseProgram();
        mShaderDualPeel->SetUniform("faceCulling", enabled);
    }
    mApplyFaceCulling = enabled;
}

void GLCanvas::SetAmbientLightIntensity(float intensity)
{
    if(mOpenGLInitialized)
    {
        mShaderDualPeel->UseProgram();
        mShaderDualPeel->SetUniform("ambientLightAmount", intensity);
    }
    mAmbientLightIntensity = intensity;
}

void GLCanvas::WillDrawBoundingBox(bool pDraw)
{
    mDrawBoundingBox = pDraw;
    updateGL();
}

void GLCanvas::WillDrawBoundingSphere(bool pDraw)
{
    mDrawBoundingSphere = pDraw;
    updateGL();
}

void GLCanvas::ApplyMaterials(bool pApplyMaterials)
{
    mApplyMaterials = pApplyMaterials;
    updateGL();
}

void GLCanvas::initializeGL()
{
    //Initialize quad where the scene is painted
    mMeshFullScreenQuad = std::make_unique<Geometry>("FullScreenQuad", GeometryTopology::Triangles);
    glm::vec2 vertices[4] = {glm::vec2(0.0f, 0.0f),
                             glm::vec2(1.0f, 0.0f),
                             glm::vec2(1.0f, 1.0f),
                             glm::vec2(0.0f, 1.0f)};
    unsigned int indices[6] = {0, 1, 2,
                               2, 3, 0};
    mMeshFullScreenQuad->SetVerticesData(4, &vertices[0]);
    mMeshFullScreenQuad->SetIndexsData(6, &indices[0]);

    mWinWidth = width();
    mWinHeight = height();

    GLenum err = glewInit();
    if(GLEW_OK != err)
    {
        const GLubyte* sError = glewGetErrorString(err);
        Debug::Log("Impossible to initialize GLEW!: " + std::string{reinterpret_cast<const char*>(sError)});
    }
    else
    {
        std::string const gl_vendor{reinterpret_cast<const char*>(glGetString(GL_VENDOR))};
        std::string const gl_renderer{reinterpret_cast<const char*>(glGetString(GL_RENDERER))};
        std::string const gl_version{reinterpret_cast<const char*>(glGetString(GL_VERSION))};
        std::string const gl_shading_language_version{reinterpret_cast<const char*>(glGetString(GL_SHADING_LANGUAGE_VERSION))};
        Debug::Log("GL_VENDOR: " + gl_vendor);
        Debug::Log("GL_RENDERER: " + gl_renderer);
        Debug::Log("GL_VERSION: " + gl_version);
        Debug::Log("GL_SHADING_LANGUAGE_VERSION: " + gl_shading_language_version);

        LoadShaders();

        // Allocate render targets first
        InitDualPeelingRenderTargets();
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        // use z-buffer
        glDisable(GL_CULL_FACE);

        glGenQueries(1, &mQueryId);
        mOpenGLInitialized = true;
        ConfigureFirstLight(mFirstLightSettings);
        ConfigureSecondLight(mSecondLightSettings);
        ApplyIllumination(mApplyIllumination);
        ApplyFaceCulling(mApplyFaceCulling);
        SetAmbientLightIntensity(mAmbientLightIntensity);
    }
}

void GLCanvas::paintGL()
{
    if(mScene != nullptr)
    {
        glDisable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);

        // ---------------------------------------------------------------------
        // 1. Initialize Min-Max Depth Buffer
        // ---------------------------------------------------------------------
        glBindFramebuffer(GL_FRAMEBUFFER, mDualPeelingSingleFboId);

        // Render targets 1 and 2 store the front and back colors
        // Clear to 0.0 and use MAX blending to filter written color
        // At most one front color and one back color can be written every pass
        glDrawBuffers(2, &mDrawBuffers[1]);
        glClearColor(0, 0, 0, 0);
        glClear(GL_COLOR_BUFFER_BIT);

        // Render target 0 stores (-minDepth, maxDepth, alphaMultiplier)
        glDrawBuffer(mDrawBuffers[0]);
        const float MAX_DEPTH = 1.0f;
        glClearColor(-MAX_DEPTH, -MAX_DEPTH, 0, 0);
        glClear(GL_COLOR_BUFFER_BIT);
        glBlendEquation(GL_MAX);

        const glm::mat4 projectionMatrix = mFreeCamera->GetProjectionMatrix();
        const glm::mat4 viewMatrix = mFreeCamera->GetViewMatrix();
        const glm::mat4 viewProjectionMatrix = projectionMatrix * viewMatrix;
        mShaderDualInit->UseProgram();
        for(int i = 0; i < mGPUScene->GetNumberOfSceneNodes(); i++)
        {
            auto sceneNode = mGPUScene->GetSceneNode(i);
            const glm::mat4 modelMatrix = sceneNode->GetModelMatrix();
            mShaderDualInit->SetUniform("modelViewProjection", viewProjectionMatrix * modelMatrix);
            //if( mWireframe )
            //    glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
            sceneNode->GetGeometry()->Draw();
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }
        DrawGeometryBoundingVolumes(viewProjectionMatrix, true);
        for (auto perVertexColorMesh : mPerVertexColorMeshes)
        {
            if( mDrawViewpointSphereInWireframe )
            {
                glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
            }
            perVertexColorMesh->Draw();
            glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
        }
        glUseProgram(0);

        CHECK_GL_ERROR();

        // ---------------------------------------------------------------------
        // 2. Dual Depth Peeling + Blending
        // ---------------------------------------------------------------------

        // Since we cannot blend the back colors in the geometry passes,
        // we use another render target to do the alpha blending
        //glBindFramebuffer(GL_FRAMEBUFFER, mDualBackBlenderFboId);
        glDrawBuffer(mDrawBuffers[6]);
        glClearColor(mBackgroundColor[0], mBackgroundColor[1], mBackgroundColor[2], 0);
        glClear(GL_COLOR_BUFFER_BIT);

        int currId = 0;
        int pass = 1;
        for (;;)
        {
            currId = pass % 2;
            int prevId = 1 - currId;
            int bufId = currId * 3;

            //glBindFramebuffer(GL_FRAMEBUFFER, mDualPeelingFboId[currId]);

            glDrawBuffers(2, &mDrawBuffers[bufId+1]);
            glClearColor(0, 0, 0, 0);
            glClear(GL_COLOR_BUFFER_BIT);

            glDrawBuffer(mDrawBuffers[bufId+0]);
            glClearColor(-MAX_DEPTH, -MAX_DEPTH, 0, 0);
            glClear(GL_COLOR_BUFFER_BIT);

            // Render target 0: RG32F MAX blending
            // Render target 1: RGBA MAX blending
            // Render target 2: RGBA MAX blending
            glDrawBuffers(3, &mDrawBuffers[bufId+0]);
            glBlendEquation(GL_MAX);

            mShaderDualPeel->UseProgram();
            mShaderDualPeel->SetUniform("world", glm::mat4(1.0f));
            mShaderDualPeel->SetUniform("cameraPosition", mFreeCamera->GetPosition());

            mShaderDualPeel->BindTexture(GL_TEXTURE_RECTANGLE, "DepthBlenderTex", mDualDepthTexId[prevId], 1);
            mShaderDualPeel->BindTexture(GL_TEXTURE_RECTANGLE, "FrontBlenderTex", mDualFrontBlenderTexId[prevId], 2);

            //Recorrem els meshos
            for(int i = 0; i < mGPUScene->GetNumberOfSceneNodes(); i++)
            {
                auto sceneNode = mGPUScene->GetSceneNode(i);
                auto currentMaterial = sceneNode->GetMaterial();
                const glm::mat4 modelMatrix = sceneNode->GetModelMatrix();
                mShaderDualPeel->SetUniform("modelViewProjection", viewProjectionMatrix * modelMatrix);
                if(mApplyMaterials && currentMaterial != nullptr)
                {
                    bool hasTexture = currentMaterial->HasKdTexture();
                    if( hasTexture )
                    {
                        auto kdTexture = currentMaterial->GetKdTexture();
                        mShaderDualPeel->BindTexture(GL_TEXTURE_2D, "diffuseTexture", kdTexture->GetGLId(), 0);
                    }
                    mShaderDualPeel->SetUniform("applyDiffuseTexture", hasTexture);
                    mShaderDualPeel->SetUniform("geometryMaterial.ka", currentMaterial->GetKa());
                    mShaderDualPeel->SetUniform("geometryMaterial.kd", currentMaterial->GetKd());
                    mShaderDualPeel->SetUniform("geometryMaterial.ks", currentMaterial->GetKs());
                    mShaderDualPeel->SetUniform("geometryMaterial.shininess", currentMaterial->GetShininess());
                }
                else
                {
                    mShaderDualPeel->SetUniform("applyDiffuseTexture", false);
                    mShaderDualPeel->SetUniform("geometryMaterial.ka", glm::vec3(0.0f, 0.0f, 0.0f));
                    mShaderDualPeel->SetUniform("geometryMaterial.kd", glm::vec3(0.6f, 0.6f, 0.6f));
                    mShaderDualPeel->SetUniform("geometryMaterial.ks", glm::vec3(0.0f, 0.0f, 0.0f));
                    mShaderDualPeel->SetUniform("geometryMaterial.shininess", 0.0f);
                }
                //if( mWireframe )
                //    glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
                sceneNode->GetGeometry()->Draw();
                glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
                //glPolygonMode( GL_FRONT, GL_FILL );
                //glPolygonMode( GL_BACK, GL_LINE );
            }
            mShaderDualPeelPerVertexColor->UseProgram();
            mShaderDualPeelPerVertexColor->BindTexture(GL_TEXTURE_RECTANGLE, "DepthBlenderTex", mDualDepthTexId[prevId], 1);
            mShaderDualPeelPerVertexColor->BindTexture(GL_TEXTURE_RECTANGLE, "FrontBlenderTex", mDualFrontBlenderTexId[prevId], 2);
            mShaderDualPeelPerVertexColor->SetUniform("modelViewProjection", viewProjectionMatrix);
            DrawGeometryBoundingVolumes(viewProjectionMatrix, false);
            for (auto perVertexColorMesh : mPerVertexColorMeshes)
            {
                if( mDrawViewpointSphereInWireframe )
                {
                    glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
                }
                perVertexColorMesh->Draw();
                glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
            }
            glUseProgram(0);

            CHECK_GL_ERROR();

            // Full screen pass to alpha-blend the back color
            glDrawBuffer(mDrawBuffers[6]);

            glBlendEquation(GL_FUNC_ADD);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

            glBeginQuery(GL_SAMPLES_PASSED, mQueryId);

            mShaderDualBlend->UseProgram();
            mShaderDualBlend->SetUniform("modelViewProjection", glm::ortho(0.0f, 1.0f, 0.0f, 1.0f));
            mShaderDualBlend->BindTexture(GL_TEXTURE_RECTANGLE, "TempTex", mDualBackTempTexId[currId], 1);
            mMeshFullScreenQuad->Draw();
            glUseProgram(0);

            CHECK_GL_ERROR();

            glEndQuery(GL_SAMPLES_PASSED);
            GLuint sample_count;
            glGetQueryObjectuiv(mQueryId, GL_QUERY_RESULT, &sample_count);
            if (sample_count == 0) {
                break;
            }

            pass++;
        }

        glDisable(GL_BLEND);

        // ---------------------------------------------------------------------
        // 3. Final Pass
        // ---------------------------------------------------------------------

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glDrawBuffer(GL_BACK);
        glClearColor(0, 0, 0, 0);
        glClear(GL_COLOR_BUFFER_BIT);

        mShaderDualFinal->UseProgram();
        mShaderDualFinal->SetUniform("modelViewProjection", glm::ortho(0.0f, 1.0f, 0.0f, 1.0f));
        mShaderDualFinal->BindTexture(GL_TEXTURE_RECTANGLE, "FrontBlenderTex", mDualFrontBlenderTexId[currId], 2);
        mShaderDualFinal->BindTexture(GL_TEXTURE_RECTANGLE, "BackBlenderTex", mDualBackBlenderTexId, 3);
        mMeshFullScreenQuad->Draw();
        glUseProgram(0);

        CHECK_GL_ERROR();
    }
    else
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClearColor(mBackgroundColor[0], mBackgroundColor[1], mBackgroundColor[2], 0);
        glClear(GL_COLOR_BUFFER_BIT);
    }
    // Flush
    glFlush();
}

void GLCanvas::resizeGL(int pWidth, int pHeight)
{
    if( pWidth != mWinWidth || pHeight != mWinHeight )
    {
        mWinHeight = pHeight;
        mWinWidth = pWidth;

        if( mFreeCamera != nullptr )
        {
            RecomputeViewport();
        }
        DeleteDualPeelingRenderTargets();
        InitDualPeelingRenderTargets();

        updateGL();
    }
}

void GLCanvas::keyPressEvent(QKeyEvent *pEvent)
{
    pEvent->ignore();
    //Tecla per recarregar els shaders
    if(pEvent->key() == Qt::Key_R)
    {
        pEvent->accept();
        LoadShaders();
        updateGL();
    }
    //Wireframe
    if(pEvent->key() == Qt::Key_F5)
    {
        pEvent->accept();
        mDrawViewpointSphereInWireframe = !mDrawViewpointSphereInWireframe;
        updateGL();
    }
    if(pEvent->key() == Qt::Key_W)
    {
        pEvent->accept();
        glm::vec3 position = mFreeCamera->GetPosition();
        glm::vec3 lookAt = mFreeCamera->GetLookAt();
        glm::vec3 front = lookAt - position;
        position += glm::normalize( front ) * mScene->GetBoundingSphere()->GetRadius() * 0.05f;

        mFreeCamera->SetPosition( position );
        updateGL();
    }
    if(pEvent->key() == Qt::Key_S)
    {
        pEvent->accept();
        glm::vec3 position = mFreeCamera->GetPosition();
        glm::vec3 lookAt = mFreeCamera->GetLookAt();
        glm::vec3 front = lookAt - position;
        position -= glm::normalize( front ) * mScene->GetBoundingSphere()->GetRadius() * 0.05f;

        mFreeCamera->SetPosition( position );
        updateGL();
    }
    if(pEvent->key() == Qt::Key_A)
    {
        pEvent->accept();
        glm::vec3 position = mFreeCamera->GetPosition();
        glm::vec3 lookAt = mFreeCamera->GetLookAt();
        glm::vec3 front = lookAt - position;
        glm::vec3 up = mFreeCamera->GetUp();
        glm::vec3 right = glm::cross( front, up );
        position -= glm::normalize( right ) * mScene->GetBoundingSphere()->GetRadius() * 0.05f;
        lookAt = position + front;

        mFreeCamera->SetPosition( position );
        mFreeCamera->SetLookAt( lookAt );
        updateGL();
    }
    if(pEvent->key() == Qt::Key_D)
    {
        pEvent->accept();
        glm::vec3 position = mFreeCamera->GetPosition();
        glm::vec3 lookAt = mFreeCamera->GetLookAt();
        glm::vec3 front = lookAt - position;
        glm::vec3 up = mFreeCamera->GetUp();
        glm::vec3 right = glm::cross( front, up );
        position += glm::normalize( right ) * mScene->GetBoundingSphere()->GetRadius() * 0.05f;
        lookAt = position + front;

        mFreeCamera->SetPosition( position );
        mFreeCamera->SetLookAt( lookAt );
        updateGL();
    }
    if(pEvent->key() == Qt::Key_Q)
    {
        pEvent->accept();
        glm::vec3 position = mFreeCamera->GetPosition();
        glm::vec3 lookAt = mFreeCamera->GetLookAt();
        glm::vec3 front = lookAt - position;
        glm::vec3 up = mFreeCamera->GetUp();
        glm::vec3 right = glm::cross( front, up );
        up = glm::cross( right, front );
        position += glm::normalize( up ) * mScene->GetBoundingSphere()->GetRadius() * 0.05f;
        lookAt = position + front;

        mFreeCamera->SetPosition( position );
        mFreeCamera->SetLookAt( lookAt );
        updateGL();
    }
    if(pEvent->key() == Qt::Key_E)
    {
        pEvent->accept();
        glm::vec3 position = mFreeCamera->GetPosition();
        glm::vec3 lookAt = mFreeCamera->GetLookAt();
        glm::vec3 front = lookAt - position;
        glm::vec3 up = mFreeCamera->GetUp();
        glm::vec3 right = glm::cross( front, up );
        up = glm::cross( right, front );
        position -= glm::normalize( up ) * mScene->GetBoundingSphere()->GetRadius() * 0.05f;
        lookAt = position + front;

        mFreeCamera->SetPosition( position );
        mFreeCamera->SetLookAt( lookAt );
        updateGL();
    }
}

void GLCanvas::DrawGeometryBoundingVolumes(const glm::mat4 &viewProjectionMatrix, bool init)
{
    if (mDrawBoundingBox || mDrawBoundingSphere)
    {
        for (auto it = mScene->cbegin(); it != mScene->cend(); ++it)
        {
            const auto &sceneNode = (*it);
            glm::mat4 modelMatrix = sceneNode.GetGlobalTransform();
            for (int i = 0; i < sceneNode.GetNumMeshes(); ++i)
            {
                if (init)
                {
                    mShaderDualInit->SetUniform("modelViewProjection", viewProjectionMatrix * modelMatrix);
                }
                else
                {
                    mShaderDualPeelPerVertexColor->SetUniform("modelViewProjection", viewProjectionMatrix * modelMatrix);
                }
                if (mDrawBoundingBox)
                {
                    sceneNode.GetMesh(i)->GetGeometry()->GetBoundingBox()->Draw();
                }
                if (mDrawBoundingSphere)
                {
                    sceneNode.GetMesh(i)->GetGeometry()->GetBoundingSphere()->Draw();
                }
            }
        }
    }
}

void GLCanvas::RecomputeViewport()
{
    float aspectRatio = mFreeCamera->GetAspectRatio();
    float widthByRatio = mWinHeight * aspectRatio;
    float heightByRatio = mWinWidth / aspectRatio;
    if( (int)(widthByRatio) > mWinWidth )
    {
        mViewport = QRect( 0, ( mWinHeight - (int)heightByRatio ) / 2, mWinWidth, (int)heightByRatio );
    }
    else if( (int)(widthByRatio) < mWinWidth )
    {
        mViewport = QRect( ( mWinWidth - (int)widthByRatio ) / 2, 0, (int)widthByRatio, mWinHeight );
    }
    else
    {
        mViewport = QRect( 0, 0, mWinWidth, mWinHeight );
    }
    glViewport( mViewport.x(), mViewport.y(), mViewport.width(), mViewport.height() );
}

void GLCanvas::LoadShaders()
{
    const GLSLShader basicVS("shaders/Basic.vert", GL_VERTEX_SHADER);
    if( basicVS.HasCompilationErrors() )
    {
        Debug::Error( "shaders/Basic.vert: " + basicVS.GetCompilationLog() );
    }
    const GLSLShader dualPeelingInitFS("shaders/DualPeelingInit.frag", GL_FRAGMENT_SHADER);
    if( dualPeelingInitFS.HasCompilationErrors() )
    {
        Debug::Error( "shaders/DualPeelingInit.frag: " + dualPeelingInitFS.GetCompilationLog() );
    }
    const GLSLShader dualPeelingPeelVS("shaders/DualPeelingPeel.vert", GL_VERTEX_SHADER);
    if( dualPeelingPeelVS.HasCompilationErrors() )
    {
        Debug::Error( "shaders/DualPeelingPeel.vert: " + dualPeelingPeelVS.GetCompilationLog() );
    }
    const GLSLShader dualPeelingPeelFS("shaders/DualPeelingPeel.frag", GL_FRAGMENT_SHADER);
    if( dualPeelingPeelFS.HasCompilationErrors() )
    {
        Debug::Error( "shaders/DualPeelingPeel.frag: " + dualPeelingPeelFS.GetCompilationLog() );
    }
    const GLSLShader shadeFragmentFS("shaders/ShadeFragment.frag", GL_FRAGMENT_SHADER);
    if( shadeFragmentFS.HasCompilationErrors() )
    {
        Debug::Error( "shaders/ShadeFragment.frag: " + shadeFragmentFS.GetCompilationLog() );
    }
    const GLSLShader shadePerVertexColorFS("shaders/ShadePerVertexColor.frag", GL_FRAGMENT_SHADER);
    if( shadePerVertexColorFS.HasCompilationErrors() )
    {
        Debug::Error( "shaders/ShadePerVertexColor.frag: " + shadePerVertexColorFS.GetCompilationLog() );
    }
    const GLSLShader dualPeelingBlendFS("shaders/DualPeelingBlend.frag", GL_FRAGMENT_SHADER);
    if( dualPeelingBlendFS.HasCompilationErrors() )
    {
        Debug::Error( "shaders/DualPeelingBlend.frag: " + dualPeelingBlendFS.GetCompilationLog() );
    }
    const GLSLShader dualPeelingFinalFS("shaders/DualPeelingFinal.frag", GL_FRAGMENT_SHADER);
    if( dualPeelingFinalFS.HasCompilationErrors() )
    {
        Debug::Error( "shaders/DualPeelingFinal.frag: " + dualPeelingFinalFS.GetCompilationLog() );
    }

    if(!basicVS.HasCompilationErrors() && !dualPeelingInitFS.HasCompilationErrors())
    {
        mShaderDualInit = std::make_unique<GLSLProgram>("ShaderDualInit");
        mShaderDualInit->AttachShader(basicVS);
        mShaderDualInit->AttachShader(dualPeelingInitFS);
        mShaderDualInit->LinkProgram();
    }
    else
    {
        mShaderDualInit = nullptr;
        Debug::Error("Could not load ShaderDualInit");
    }

    if(!dualPeelingPeelVS.HasCompilationErrors() && !dualPeelingPeelFS.HasCompilationErrors() && !shadeFragmentFS.HasCompilationErrors())
    {
        mShaderDualPeel = std::make_unique<GLSLProgram>("ShaderDualPeel");
        mShaderDualPeel->AttachShader(dualPeelingPeelVS);
        mShaderDualPeel->AttachShader(dualPeelingPeelFS);
        mShaderDualPeel->AttachShader(shadeFragmentFS);
        mShaderDualPeel->LinkProgram();
    }
    else
    {
        mShaderDualPeel = nullptr;
        Debug::Error("Could not load ShaderDualPeel");
    }

    if(!dualPeelingPeelVS.HasCompilationErrors() && !dualPeelingPeelFS.HasCompilationErrors() && !shadePerVertexColorFS.HasCompilationErrors())
    {
        mShaderDualPeelPerVertexColor = std::make_unique<GLSLProgram>("ShaderDualPeelPerVertexColor");
        mShaderDualPeelPerVertexColor->AttachShader(dualPeelingPeelVS);
        mShaderDualPeelPerVertexColor->AttachShader(dualPeelingPeelFS);
        mShaderDualPeelPerVertexColor->AttachShader(shadePerVertexColorFS);
        mShaderDualPeelPerVertexColor->LinkProgram();
    }
    else
    {
        mShaderDualPeelPerVertexColor = nullptr;
        Debug::Error("Could not load ShaderDualPeelPerVertexColor");
    }

    if(!basicVS.HasCompilationErrors() && !dualPeelingBlendFS.HasCompilationErrors())
    {
        mShaderDualBlend = std::make_unique<GLSLProgram>("ShaderDualBlend");
        mShaderDualBlend->AttachShader(basicVS);
        mShaderDualBlend->AttachShader(dualPeelingBlendFS);
        mShaderDualBlend->LinkProgram();
    }
    else
    {
        mShaderDualBlend = nullptr;
        Debug::Error("Could not load ShaderDualBlend");
    }

    if(!basicVS.HasCompilationErrors() && !dualPeelingFinalFS.HasCompilationErrors())
    {
        mShaderDualFinal = std::make_unique<GLSLProgram>("ShaderDualFinal");
        mShaderDualFinal->AttachShader(basicVS);
        mShaderDualFinal->AttachShader(dualPeelingFinalFS);
        mShaderDualFinal->LinkProgram();
    }
    else
    {
        mShaderDualFinal = nullptr;
        Debug::Error("Could not load ShaderDualFinal");
    }
}

void GLCanvas::InitDualPeelingRenderTargets()
{
    glGenTextures(2, mDualDepthTexId);
    glGenTextures(2, mDualFrontBlenderTexId);
    glGenTextures(2, mDualBackTempTexId);
    glGenFramebuffers(1, &mDualPeelingSingleFboId);
    for(int i = 0; i < 2; i++)
    {
        glBindTexture(GL_TEXTURE_RECTANGLE, mDualDepthTexId[i]);
        glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexImage2D(GL_TEXTURE_RECTANGLE, 0, GL_RG32F, mWinWidth, mWinHeight, 0, GL_RG, GL_FLOAT, 0);

        glBindTexture(GL_TEXTURE_RECTANGLE, mDualFrontBlenderTexId[i]);
        glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexImage2D(GL_TEXTURE_RECTANGLE, 0, GL_RGBA, mWinWidth, mWinHeight, 0, GL_RGBA, GL_FLOAT, 0);

        glBindTexture(GL_TEXTURE_RECTANGLE, mDualBackTempTexId[i]);
        glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexImage2D(GL_TEXTURE_RECTANGLE, 0, GL_RGBA, mWinWidth, mWinHeight, 0, GL_RGBA, GL_FLOAT, 0);
    }

    glGenTextures(1, &mDualBackBlenderTexId);
    glBindTexture(GL_TEXTURE_RECTANGLE, mDualBackBlenderTexId);
    glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_RECTANGLE, 0, GL_RGB, mWinWidth, mWinHeight, 0, GL_RGB, GL_FLOAT, 0);

    glGenFramebuffers(1, &mDualBackBlenderFboId);
    glBindFramebuffer(GL_FRAMEBUFFER, mDualBackBlenderFboId);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_RECTANGLE, mDualBackBlenderTexId, 0);

    glBindFramebuffer(GL_FRAMEBUFFER, mDualPeelingSingleFboId);

    int j = 0;
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_RECTANGLE, mDualDepthTexId[j], 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_RECTANGLE, mDualFrontBlenderTexId[j], 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_RECTANGLE, mDualBackTempTexId[j], 0);

    j = 1;
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_RECTANGLE, mDualDepthTexId[j], 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT4, GL_TEXTURE_RECTANGLE, mDualFrontBlenderTexId[j], 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT5, GL_TEXTURE_RECTANGLE, mDualBackTempTexId[j], 0);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT6, GL_TEXTURE_RECTANGLE, mDualBackBlenderTexId, 0);

    CHECK_GL_ERROR();
}

void GLCanvas::DeleteDualPeelingRenderTargets()
{
    glDeleteFramebuffers(1, &mDualBackBlenderFboId);

    glDeleteTextures(1, &mDualBackBlenderTexId);
    glDeleteFramebuffers(1, &mDualPeelingSingleFboId);
    glDeleteTextures(2, mDualBackTempTexId);
    glDeleteTextures(2, mDualFrontBlenderTexId);
    glDeleteTextures(2, mDualDepthTexId);
}
