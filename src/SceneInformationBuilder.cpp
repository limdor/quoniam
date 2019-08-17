//Definition include
#include "SceneInformationBuilder.h"

//GLEW has to be included before any OpenGL include
#include "glew.h"

//Qt includes
#include <QApplication>
#include <QProgressDialog>
#include <QTime>
#include <QVector>

//Dependency includes
#include "glm/mat4x4.hpp"
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include "glm/common.hpp"
#include "glm/geometric.hpp"
#include "glm/trigonometric.hpp"
#include "glm/gtx/vector_angle.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"

//Project includes
#include "Debug.h"
#include "GLSLProgram.h"
#include "GLSLShader.h"
#include "GPUGeometry.h"
#include "GPUScene.h"
#include "MainWindow.h"

SceneInformationBuilder::SceneInformationBuilder():
    mSerializedScene(nullptr), mProjectedAreasMatrix(nullptr), mWidthResolution(640), mAspectRatio(1.0f),
    mPreviousDepthTest(true), mPreviousCullFace(true), mPreviousBlend(false)
{
}

SceneInformationBuilder::~SceneInformationBuilder()
{
    delete mSerializedScene;
    delete mProjectedAreasMatrix;
}

void SceneInformationBuilder::CreateHistogram(Scene* pScene, SphereOfViewpoints* pSphereOfViewpoints, int pWidthResolution, bool pFaceCulling, bool pIgnoreNormals)
{
    int numberOfViewpoints = pSphereOfViewpoints->GetNumberOfViewpoints();

    QApplication::setOverrideCursor( Qt::WaitCursor );
    QTime t;
    t.start();
    QProgressDialog progress(MainWindow::GetInstance());
    progress.setLabelText("Projecting scene to viewpoint sphere...");
    progress.setCancelButton(nullptr);
    progress.setRange(0, numberOfViewpoints);
    progress.show();

    SaveOpenGLStats();

    glEnable(GL_DEPTH_TEST);
    if(pFaceCulling && !pIgnoreNormals)
    {
        glEnable(GL_CULL_FACE);
    }
    else
    {
        glDisable(GL_CULL_FACE);
    }
    glDisable(GL_BLEND);

    GLuint renderBuffer, frameBuffer, depthRenderTexture;
    //Create RenderBuffer
    glGenRenderbuffers( 1, &renderBuffer );
    glGenTextures( 1, &depthRenderTexture );
    //Create FrameBuffer
    glGenFramebuffers( 1, &frameBuffer );
    glBindFramebuffer( GL_FRAMEBUFFER, frameBuffer );
    CHECK_GL_ERROR();

    if(mShaderColorPerFace == nullptr)
    {
        GLSLShader basicVS{"shaders/Basic.vert", GL_VERTEX_SHADER};
        if( basicVS.HasErrors() )
        {
            Debug::Error( QString("shaders/Basic.vert: %1").arg(basicVS.GetLog()) );
        }
        GLSLShader colorPerFaceFS{"shaders/ColorPerFace.frag", GL_FRAGMENT_SHADER};
        if( colorPerFaceFS.HasErrors() )
        {
            Debug::Error( QString("shaders/ColorPerFace.frag: %1").arg(colorPerFaceFS.GetLog()) );
        }

        mShaderColorPerFace = std::make_unique<GLSLProgram>("ShaderColorPerFace");
        mShaderColorPerFace->AttachShader(basicVS);
        mShaderColorPerFace->AttachShader(colorPerFaceFS);
        mShaderColorPerFace->BindFragDataLocation(0, "outputColor");
        mShaderColorPerFace->LinkProgram();
    }

    //Initialization to be able to draw
    mShaderColorPerFace->UseProgram();
    mShaderColorPerFace->SetUniform("ignoreNormals", pIgnoreNormals);
    glDrawBuffer(GL_COLOR_ATTACHMENT0);
    glReadBuffer(GL_COLOR_ATTACHMENT0);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    mAspectRatio = pSphereOfViewpoints->GetAspectRatio();
    mWidthResolution = pWidthResolution;
    int windowWidth = pWidthResolution;
    int windowHeight = static_cast<int>(windowWidth / mAspectRatio);

    //RenderBuffer configurations
    glBindRenderbuffer( GL_RENDERBUFFER, renderBuffer );
    glRenderbufferStorage( GL_RENDERBUFFER, GL_R32F, windowWidth, windowHeight );
    CHECK_GL_ERROR();

    glBindTexture( GL_TEXTURE_2D, depthRenderTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, windowWidth, windowHeight, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, nullptr);
    CHECK_GL_ERROR();

    glFramebufferRenderbuffer( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, renderBuffer);
    CHECK_GL_ERROR();
    glFramebufferTexture( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthRenderTexture, 0);
    CHECK_GL_ERROR();
#ifdef QT_DEBUG
    if( glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        CHECK_GL_ERROR();
#endif

    //Compute the total number of pixels
    unsigned int totalNumberOfPixels = ((unsigned int)windowWidth * (unsigned int)windowHeight);

    //Storage for the pixels on every draw
    float* valuePerFaceImage = new float [totalNumberOfPixels];
    GLubyte* depthImage = new GLubyte [totalNumberOfPixels];
    unsigned char* projectionMask = new unsigned char[totalNumberOfPixels];

    glViewport( 0, 0, windowWidth, windowHeight );

    delete mSerializedScene;
    delete mProjectedAreasMatrix;
    mViewpointNeighbours = pSphereOfViewpoints->GetNeighbours();
    mSerializedScene = new SerializedSceneGeometry(pScene);
    int numberOfPolygons = pScene->GetNumberOfPolygons();
    QVector< unsigned int > facesAreas(numberOfPolygons);
    mProjectedAreasMatrix = new ProjectedAreasMatrix(numberOfViewpoints, numberOfPolygons);
    mSilhouetteLengths.resize(numberOfViewpoints);
    mSilhouetteCurvature.resize(numberOfViewpoints);
    mNormalizedDepthHistograms.resize(numberOfViewpoints);
    mMaxDepths.resize(numberOfViewpoints);
    mDepthImages.resize(numberOfViewpoints);
    mVisibleVertexs.resize(numberOfViewpoints);
    GPUScene* gpuScene = new GPUScene(pScene);
    Geometry* verticesScene = new Geometry("Vertices", Geometry::Points);
    QVector<glm::vec3> vertices = mSerializedScene->GetVertices();
    int numberOfVertices = vertices.size();
    QVector<unsigned int> indexs(numberOfVertices);
    for( int j = 0; j < numberOfVertices; j++ )
    {
        indexs[j] = j;
    }
    verticesScene->SetVerticesData(numberOfVertices, vertices.data());
    verticesScene->SetIndexsData(numberOfVertices, indexs.data());
    //We iterate over all the viewpoint of the sphere
    for( int i = 0; i < numberOfViewpoints; i++ )
    {
        //We update the progress bar
        progress.setValue(i);   

        //We compute the camera matrix
        Camera* currentViewpoint = pSphereOfViewpoints->GetViewpoint(i);
        glm::mat4 viewMatrix = currentViewpoint->GetViewMatrix();
        glm::mat4 projectionMatrix = currentViewpoint->GetProjectionMatrix();
        glm::mat4 viewProjectionMatrix = projectionMatrix * viewMatrix;

        //We clean the color and the depth buffer
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

        //We paint the scene using a different color per face
        for( int k = 0; k < gpuScene->GetNumberOfSceneNodes(); k++ )
        {
            GPUSceneNode* sceneNode = gpuScene->GetSceneNode(k);
            glm::mat4 modelMatrix = sceneNode->GetModelMatrix();
            mShaderColorPerFace->SetUniform("modelViewProjection", viewProjectionMatrix * modelMatrix);
            mShaderColorPerFace->SetUniform("offset", sceneNode->GetPolygonalOffset());
            sceneNode->GetGeometry()->Draw();
        }
        glFlush();

        //We get the rendered image
        glReadPixels(0, 0, windowWidth, windowHeight, GL_RED, GL_FLOAT, valuePerFaceImage);
        CHECK_GL_ERROR();

        //We compute the projected area of every face and we compute a mask to know what is model and what is background
        facesAreas.fill( 0 );
        for(unsigned int j = 0; j < totalNumberOfPixels; j++ )
        {
            int pixelActual = glm::round(valuePerFaceImage[j]);

            if(pixelActual > 0)
            {
                Q_ASSERT(pixelActual <= numberOfPolygons);
                facesAreas[pixelActual - 1]++;
                projectionMask[j] = 255;
            }
            else
            {
                projectionMask[j] = 0;
            }
        }
        mProjectedAreasMatrix->SetValues(i, facesAreas);

        //We convert the mask to OpenCV format
        cv::Mat image(windowHeight, windowWidth, CV_8UC1, projectionMask);
        cv::flip(image, image, 0);

        //We obtain the depth image
        glBindTexture( GL_TEXTURE_2D, depthRenderTexture);
        glGetTexImage(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, depthImage);
        CHECK_GL_ERROR();
        cv::Mat imageDepth(windowHeight, windowWidth, CV_8UC1, depthImage);
        cv::flip(imageDepth, imageDepth, 0);

        mDepthImages[i] = imageDepth.clone();

        //Compute the normalized depth histogram
        //TODO: Make sure that the depth is properly adjusted
        cv::Mat hist;
        int histSize = 256;
        float range[] = { 0, 256 } ;
        const float* histRange = { range };
        cv::calcHist(&imageDepth, 1, 0, image, hist, 1, &histSize, &histRange, true, false);
        mNormalizedDepthHistograms[i].resize(histSize);
        int nonZero = cv::countNonZero(image);
        for( int j = 0; j < histSize; j++ )
        {
            mNormalizedDepthHistograms[i][j] = hist.at<float>(j) / nonZero;
        }

        //Compute the maximum depth
        double maximum;
        cv::minMaxLoc(imageDepth, nullptr, &maximum, nullptr, nullptr, image);
        mMaxDepths[i] = (float)maximum / 255;

        //Compute the silhouette length and the silhouette curvature
        std::vector<std::vector<cv::Point> > contours;
        std::vector<cv::Vec4i> hierarchy;
        cv::findContours(image, contours, hierarchy, CV_RETR_LIST, CV_CHAIN_APPROX_NONE);
        mSilhouetteLengths[i] = 0.0f;
        unsigned int numberOfAngles = 0;
        for( int j = 0; j < contours.size(); j++ )
        {
            mSilhouetteLengths[i] += cv::arcLength( contours.at(j), true );
            unsigned int contourSize = (unsigned int)contours.at(j).size();
            mSilhouetteCurvature[i].resize(numberOfAngles + contourSize);
            for( unsigned int k = 0; k < contourSize; k++ )
            {
                cv::Point diff1 = contours.at(j).at(k) - contours.at(j).at((k + 1) % contourSize);
                cv::Point diff2 = contours.at(j).at((k + 1) % contourSize) - contours.at(j).at((k + 2) % contourSize);
                float angle = 0.0f;
                if(diff1.x != diff2.x || diff1.y != diff2.y)
                {
                    glm::vec2 v1(diff1.x, diff1.y);
                    glm::vec2 v2(diff2.x, diff2.y);
                    v1 = glm::normalize(v1);
                    v2 = glm::normalize(v2);
                    angle = glm::degrees(glm::orientedAngle(v1, v2));
                }
                mSilhouetteCurvature[i][numberOfAngles + k] = angle;
            }
            numberOfAngles += contourSize;
        }

        //We compute the visibility of every vertex
        glClear( GL_COLOR_BUFFER_BIT );
        mShaderColorPerFace->SetUniform("modelViewProjection", viewProjectionMatrix);
        mShaderColorPerFace->SetUniform("offset", 0);
        verticesScene->Draw();
        glReadPixels(0, 0, windowWidth, windowHeight, GL_RED, GL_FLOAT, valuePerFaceImage);
        CHECK_GL_ERROR();
        QSet<int> visibleVertexs;
        for(unsigned int j = 0; j < totalNumberOfPixels; j++ )
        {
            int pixelActual = glm::round(valuePerFaceImage[j]);

            if(pixelActual > 0)
            {
                Q_ASSERT(pixelActual <= numberOfVertices);
                visibleVertexs.insert(pixelActual-1);
            }
        }
        mVisibleVertexs[i] = visibleVertexs;
    }
    delete verticesScene;
    delete gpuScene;
    delete[] projectionMask;
    delete[] depthImage;
    delete[] valuePerFaceImage;
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    //Delete framebuffer and the two render buffers
    glDeleteFramebuffers( 1, &frameBuffer);
    glDeleteRenderbuffers( 1, &depthRenderTexture);
    glDeleteRenderbuffers( 1, &renderBuffer );

    mProjectedAreasMatrix->Compute();
    Debug::Log( QString("GLCanvas::ComputeViewpointsProbabilities %1x%2 - Time elapsed: %3 ms").arg(windowWidth).arg(windowHeight).arg(t.elapsed()) );

    RestoreOpenGLStats();

    progress.hide();

    QApplication::restoreOverrideCursor();
}

const ProjectedAreasMatrix *SceneInformationBuilder::GetProjectedAreasMatrix() const
{
    return mProjectedAreasMatrix;
}

QVector< QVector< int > > SceneInformationBuilder::GetViewpointNeighbours() const
{
    return mViewpointNeighbours;
}

QVector< QVector< int > > SceneInformationBuilder::GetSerializedPolygonNeighbours() const
{
    return mSerializedScene->GetFacesNeighbours();
}

float SceneInformationBuilder::GetSilhouetteLength(int pViewpoint) const
{
    return mSilhouetteLengths.at(pViewpoint);
}

QVector< float > SceneInformationBuilder::GetSilhouetteCurvature(int pViewpoint) const
{
    return mSilhouetteCurvature.at(pViewpoint);
}

QVector< float > SceneInformationBuilder::GetNormalizedDepthHistogram(int pViewpoint) const
{
    return mNormalizedDepthHistograms.at(pViewpoint);
}

cv::Mat SceneInformationBuilder::GetDepthImage(int pViewpoint) const
{
    return mDepthImages.at(pViewpoint);
}

float SceneInformationBuilder::GetMaximumDepth(int pViewpoint) const
{
    return mMaxDepths.at(pViewpoint);
}

QSet< int > SceneInformationBuilder::GetVisibleVertices(int pViewpoint) const
{
    return mVisibleVertexs.at(pViewpoint);
}

QVector< float > SceneInformationBuilder::GetSerializedPolygonAreas() const
{
    return mSerializedScene->GetFacesAreas();
}

QVector< float > SceneInformationBuilder::GetSerializedVertexCurvature() const
{
    return mSerializedScene->GetVerticesCurvature();
}

int SceneInformationBuilder::GetWidthResolution() const
{
    return mWidthResolution;
}

float SceneInformationBuilder::GetAspectRatio() const
{
    return mAspectRatio;
}

void SceneInformationBuilder::SaveOpenGLStats()
{
    mPreviousDepthTest = glIsEnabled(GL_DEPTH_TEST);
    mPreviousCullFace = glIsEnabled(GL_CULL_FACE);
    mPreviousBlend = glIsEnabled(GL_BLEND);
    glGetFloatv(GL_COLOR_CLEAR_VALUE, &mPreviousClearColor[0]);
    glGetIntegerv(GL_VIEWPORT, &mPreviousViewport[0]);
}

void SceneInformationBuilder::RestoreOpenGLStats()
{
    if(mPreviousDepthTest)
    {
        glEnable(GL_DEPTH_TEST);
    }
    else
    {
        glDisable(GL_DEPTH_TEST);
    }
    if(mPreviousCullFace)
    {
        glEnable(GL_CULL_FACE);
    }
    else
    {
        glDisable(GL_CULL_FACE);
    }
    if(mPreviousBlend)
    {
        glEnable(GL_BLEND);
    }
    else
    {
        glDisable(GL_BLEND);
    }
    glClearColor(mPreviousClearColor.r, mPreviousClearColor.g, mPreviousClearColor.b, mPreviousClearColor.a);
    glViewport(mPreviousViewport[0], mPreviousViewport[1], mPreviousViewport[2], mPreviousViewport[3]);
}
