//Definition include
#include "GLCanvas.h"

//Dependency includes
#include "glm/mat4x4.hpp"
#include "glm/vec2.hpp"
#include "glm/geometric.hpp"
#include "glm/gtc/matrix_transform.hpp"

//Project includes
#include "Debug.h"
#include "GPUGeometry.h"
#include "OrthographicCamera.h"
#include "Tools.h"

GLCanvas::GLCanvas(QWidget *pParent): QGLWidget(QGLFormat(QGL::SampleBuffers), pParent),
    mDrawBoundingBox(false), mDrawBoundingSphere(false), mDrawWireframe(false), mApplyMaterials(true),
    mFreeCamera(NULL), mWinWidth(0), mWinHeight(0),
    mScene(NULL), mGPUScene(NULL),
    mShaderDualInit(NULL), mShaderDualPeel(NULL), mShaderDualPeelPerVertexColor(NULL), mShaderDualBlend(NULL), mShaderDualFinal(NULL),
    mBackgroundColor(0.5f, 0.5f, 0.5f)
{
    //Inicialització dels draw buffers
    mDrawBuffers[0] = GL_COLOR_ATTACHMENT0;
    mDrawBuffers[1] = GL_COLOR_ATTACHMENT1;
    mDrawBuffers[2] = GL_COLOR_ATTACHMENT2;
    mDrawBuffers[3] = GL_COLOR_ATTACHMENT3;
    mDrawBuffers[4] = GL_COLOR_ATTACHMENT4;
    mDrawBuffers[5] = GL_COLOR_ATTACHMENT5;
    mDrawBuffers[6] = GL_COLOR_ATTACHMENT6;

    //Inicialització del quad on es pinta l'escena
    mMeshFullScreenQuad = new Geometry("FullScreenQuad", Geometry::Triangles);
    glm::vec2 vertices[4] = {glm::vec2(0.0f, 0.0f),
                             glm::vec2(1.0f, 0.0f),
                             glm::vec2(1.0f, 1.0f),
                             glm::vec2(0.0f, 1.0f)};
    unsigned int indices[6] = {0, 1, 2,
                               2, 3, 0};
    mMeshFullScreenQuad->SetVerticesData(4, &vertices[0]);
    mMeshFullScreenQuad->SetIndexsData(6, &indices[0]);
}

GLCanvas::~GLCanvas()
{
    //Destrucció dels framebuffers i textures per fer el dual depth peeling
    DeleteDualPeelingRenderTargets();

    //Destrucció dels shaders
    DeleteShaders();

    glDeleteQueries(1, &mQueryId);

    delete mMeshFullScreenQuad;
    if(mGPUScene != NULL)
    {
        delete mGPUScene;
        delete mFreeCamera;
    }
}

void GLCanvas::LoadScene(Scene* pScene, const Camera* pCamera)
{
    makeCurrent();
    mScene = pScene;
    if(mGPUScene != NULL)
    {
        delete mGPUScene;
    }
    mGPUScene = new GPUScene(pScene);
    mPerVertexColorMeshes.clear();

    if( mFreeCamera != NULL )
    {
        delete mFreeCamera;
    }
    if(pCamera != NULL)
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
        mFreeCamera = new PerspectiveCamera(0.05f * radius, radius * 50.0f, center, up, position, 60.0f, mWinWidth/(float)mWinHeight);
    }

    RecomputeViewport();
    updateGL();
}

void GLCanvas::SetPerVertexMesh(Geometry *pPerVertexMesh)
{
    mPerVertexColorMeshes.clear();
    mPerVertexColorMeshes.push_back(pPerVertexMesh);
}

void GLCanvas::AddPerVertexMesh(Geometry* pPerVertexMesh)
{
    mPerVertexColorMeshes.push_back(pPerVertexMesh);
}

GLSLProgram* GLCanvas::GetShaderProgram() const
{
    return mShaderDualPeel;
}

QString GLCanvas::SaveScreenshot( const QString &pFileName )
{
    makeCurrent();
    QString completePath = Tools::GetProgramPath();
    QImage image = grabFrameBuffer();
    if(!image.save( completePath + pFileName ))
    {
        Debug::Warning( QString("Screenshot have not been saved: %1").arg( completePath + pFileName ) );
    }
    return (completePath + pFileName);
}

void GLCanvas::SetCamera(const Camera* pCamera)
{
    makeCurrent();
    if( mFreeCamera != NULL )
    {
        delete mFreeCamera;
    }
    mFreeCamera = pCamera->Clone();
    RecomputeViewport();
    updateGL();
}

Camera* GLCanvas::GetCamera()
{
    return mFreeCamera;
}

Scene* GLCanvas::GetScene()
{
    return mScene;
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
    mWinWidth = width();
    mWinHeight = height();

    GLenum err = glewInit();
    if(GLEW_OK != err)
    {
        const GLubyte* sError = glewGetErrorString(err);
        Debug::Log(QString("Impossible to initialize GLEW!: %1").arg(QString((const char*)sError)));
    }
    else
    {
        LoadShaders();
        Debug::Log(QString("GL_VENDOR: %1").arg(QString((const char *)glGetString(GL_VENDOR))));
        Debug::Log(QString("GL_RENDERER: %1").arg(QString((const char *)glGetString(GL_RENDERER))));
        Debug::Log(QString("GL_VERSION: %1").arg(QString((const char *)glGetString(GL_VERSION))));
        Debug::Log(QString("GL_SHADING_LANGUAGE_VERSION: %1").arg(QString((const char *)glGetString(GL_SHADING_LANGUAGE_VERSION))));
        //GLint numberOfExtensions;
        //glGetIntegerv(GL_NUM_EXTENSIONS, &numberOfExtensions);
        //QString extensions = "";
        //for( int i = 0; i < numberOfExtensions; i++ )
        //{
        //    extensions += QString("%1, ").arg((const char *)glGetStringi(GL_EXTENSIONS, i));
        //}
        //Debug::Log(QString("GL_EXTENSIONS: %1").arg(extensions));

        // Allocate render targets first
        InitDualPeelingRenderTargets();
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        // use z-buffer
        glDisable(GL_CULL_FACE);

        glGenQueries(1, &mQueryId);
    }
}

void GLCanvas::paintGL()
{
    if(mScene != NULL)
    {
        const float MAX_DEPTH = 1.0f;

        glm::mat4 projectionMatrix = mFreeCamera->GetProjectionMatrix();
        glm::mat4 viewMatrix = mFreeCamera->GetViewMatrix();
        glm::mat4 viewProjectionMatrix = projectionMatrix * viewMatrix;

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
        glClearColor(-MAX_DEPTH, -MAX_DEPTH, 0, 0);
        glClear(GL_COLOR_BUFFER_BIT);
        glBlendEquation(GL_MAX);

        mShaderDualInit->UseProgram();
        for(int i = 0; i < mGPUScene->GetNumberOfSceneNodes(); i++)
        {
            GPUSceneNode* sceneNode = mGPUScene->GetSceneNode(i);
            glm::mat4 modelMatrix = sceneNode->GetModelMatrix();
            mShaderDualInit->SetUniform("modelViewProjection", viewProjectionMatrix * modelMatrix);
            //if( mWireframe )
            //    glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
            sceneNode->GetGeometry()->Draw();
            glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
 //           if(mDrawBoundingBox)
 //               mScene->GetMesh(i)->GetGeometry()->GetBoundingBox()->Draw();
 //           if(mDrawBoundingSphere)
 //               mScene->GetMesh(i)->GetGeometry()->GetBoundingSphere()->Draw();
        }
        for(int i = 0; i < mPerVertexColorMeshes.size(); i++)
        {
            if( mDrawWireframe )
            {
                glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
            }
            mPerVertexColorMeshes.at(i)->Draw();
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
            mShaderDualPeel->SetUniform("world", glm::mat4());
            mShaderDualPeel->SetUniform("cameraPosition", mFreeCamera->GetPosition());

            mShaderDualPeel->BindTexture(GL_TEXTURE_RECTANGLE, "DepthBlenderTex", mDualDepthTexId[prevId], 1);
            mShaderDualPeel->BindTexture(GL_TEXTURE_RECTANGLE, "FrontBlenderTex", mDualFrontBlenderTexId[prevId], 2);

            //Recorrem els meshos
            for(int i = 0; i < mGPUScene->GetNumberOfSceneNodes(); i++)
            {
                GPUSceneNode* sceneNode = mGPUScene->GetSceneNode(i);
                Material* currentMaterial = sceneNode->GetMaterial();
                glm::mat4 modelMatrix = sceneNode->GetModelMatrix();
                mShaderDualPeel->SetUniform("modelViewProjection", viewProjectionMatrix * modelMatrix);
                if(mApplyMaterials && currentMaterial != NULL)
                {
                    bool hasTexture = currentMaterial->HasKdTexture();
                    if( hasTexture )
                    {
                        Texture* kdTexture = currentMaterial->GetKdTexture();
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
            DrawGeometryBoundingVolumes();
            for(int i = 0; i < mPerVertexColorMeshes.size(); i++)
            {
                if( mDrawWireframe )
                {
                    glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
                }
                mPerVertexColorMeshes.at(i)->Draw();
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

        if( mFreeCamera != NULL )
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
        DeleteShaders();
        LoadShaders();
        updateGL();
    }
    //Wireframe
    if(pEvent->key() == Qt::Key_F5)
    {
        pEvent->accept();
        mDrawWireframe = !mDrawWireframe;
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

void GLCanvas::DrawGeometryBoundingVolumes()
{
//    for(int i = 0; i < mScene->GetNumberOfMeshes(); i++)
//    {
//        if(mDrawBoundingBox)
//            mScene->GetMesh(i)->GetGeometry()->GetBoundingBox()->Draw();
//        if(mDrawBoundingSphere)
//            mScene->GetMesh(i)->GetGeometry()->GetBoundingSphere()->Draw();
//    }
//    CHECK_GL_ERROR();
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
    GLSLShader* basicVS = new GLSLShader("shaders/Basic.vert", GL_VERTEX_SHADER);
    if( basicVS->HasErrors() )
    {
        Debug::Error( QString("shaders/Basic.vert: %1").arg( basicVS->GetLog( )) );
    }
    GLSLShader* dualPeelingInitFS = new GLSLShader("shaders/DualPeelingInit.frag", GL_FRAGMENT_SHADER);
    if( dualPeelingInitFS->HasErrors() )
    {
        Debug::Error( QString("shaders/DualPeelingInit.frag: %1").arg(dualPeelingInitFS->GetLog()) );
    }
    GLSLShader* dualPeelingPeelVS = new GLSLShader("shaders/DualPeelingPeel.vert", GL_VERTEX_SHADER);
    if( dualPeelingPeelVS->HasErrors() )
    {
        Debug::Error( QString("shaders/DualPeelingPeel.vert: %1").arg(dualPeelingPeelVS->GetLog()) );
    }
    GLSLShader* dualPeelingPeelFS = new GLSLShader("shaders/DualPeelingPeel.frag", GL_FRAGMENT_SHADER);
    if( dualPeelingPeelFS->HasErrors() )
    {
        Debug::Error( QString("shaders/DualPeelingPeel.frag: %1").arg(dualPeelingPeelFS->GetLog()) );
    }
    GLSLShader* shadeFragmentFS = new GLSLShader("shaders/ShadeFragment.frag", GL_FRAGMENT_SHADER);
    if( shadeFragmentFS->HasErrors() )
    {
        Debug::Error( QString("shaders/ShadeFragment.frag: %1").arg(shadeFragmentFS->GetLog()) );
    }
    GLSLShader* shadePerVertexColorFS = new GLSLShader("shaders/ShadePerVertexColor.frag", GL_FRAGMENT_SHADER);
    if( shadePerVertexColorFS->HasErrors() )
    {
        Debug::Error( QString("shaders/ShadePerVertexColor.frag: %1").arg(shadePerVertexColorFS->GetLog()) );
    }
    GLSLShader* dualPeelingBlendFS = new GLSLShader("shaders/DualPeelingBlend.frag", GL_FRAGMENT_SHADER);
    if( dualPeelingBlendFS->HasErrors() )
    {
        Debug::Error( QString("shaders/DualPeelingBlend.frag: %1").arg(dualPeelingBlendFS->GetLog()) );
    }
    GLSLShader* dualPeelingFinalFS = new GLSLShader("shaders/DualPeelingFinal.frag", GL_FRAGMENT_SHADER);
    if( dualPeelingFinalFS->HasErrors() )
    {
        Debug::Error( QString("shaders/DualPeelingFinal.frag: %1").arg(dualPeelingFinalFS->GetLog()) );
    }

    mShaderDualInit = new GLSLProgram("ShaderDualInit");
    mShaderDualInit->AttachShader(basicVS);
    mShaderDualInit->AttachShader(dualPeelingInitFS);
    mShaderDualInit->LinkProgram();

    mShaderDualPeel = new GLSLProgram("ShaderDualPeel");
    mShaderDualPeel->AttachShader(dualPeelingPeelVS);
    mShaderDualPeel->AttachShader(dualPeelingPeelFS);
    mShaderDualPeel->AttachShader(shadeFragmentFS);
    mShaderDualPeel->LinkProgram();

    mShaderDualPeelPerVertexColor = new GLSLProgram("ShaderDualPeelPerVertexColor");
    mShaderDualPeelPerVertexColor->AttachShader(dualPeelingPeelVS);
    mShaderDualPeelPerVertexColor->AttachShader(dualPeelingPeelFS);
    mShaderDualPeelPerVertexColor->AttachShader(shadePerVertexColorFS);
    mShaderDualPeelPerVertexColor->LinkProgram();

    mShaderDualBlend = new GLSLProgram("ShaderDualBlend");
    mShaderDualBlend->AttachShader(basicVS);
    mShaderDualBlend->AttachShader(dualPeelingBlendFS);
    mShaderDualBlend->LinkProgram();

    mShaderDualFinal = new GLSLProgram("ShaderDualFinal");
    mShaderDualFinal->AttachShader(basicVS);
    mShaderDualFinal->AttachShader(dualPeelingFinalFS);
    mShaderDualFinal->LinkProgram();

    delete basicVS;
    delete dualPeelingInitFS;
    delete dualPeelingPeelVS;
    delete dualPeelingPeelFS;
    delete shadeFragmentFS;
    delete shadePerVertexColorFS;
    delete dualPeelingBlendFS;
    delete dualPeelingFinalFS;
}

void GLCanvas::DeleteShaders()
{
    delete mShaderDualInit;
    delete mShaderDualPeel;
    delete mShaderDualPeelPerVertexColor;
    delete mShaderDualBlend;
    delete mShaderDualFinal;
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
