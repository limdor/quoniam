/// \file SceneInformationBuilder.h
/// \class SceneInformationBuilder
/// \author Xavier Bonaventura
/// \author Copyright: (c) Universitat de Girona

#ifndef SCENE_INFORMATION_BUILDER_H
#define SCENE_INFORMATION_BUILDER_H

//Dependency includes
#include "glm/vec4.hpp"
#include "opencv2/core/core.hpp"

//Project includes
#include "GLSLProgram.h"
#include "ProjectedAreasMatrix.h"
#include "Scene.h"
#include "SerializedSceneGeometry.h"
#include "SphereOfViewpoints.h"

/// Static class to create an InformationChannelHistogram
class SceneInformationBuilder
{
public:
    /// Constructor
    SceneInformationBuilder();
    /// Destructor
    ~SceneInformationBuilder();

    /// Create an InformationChannelHistogram given the Scene and the SphereOfViewpoints
    void CreateHistogram(Scene* pScene, SphereOfViewpoints* pSphereOfViewpoints, int pWidthResolution, bool pFaceCulling, bool pIgnoreNormals = false);
    /// Get the histogram
    const ProjectedAreasMatrix* GetProjectedAreasMatrix() const;
    QVector< QVector< int > > GetViewpointNeighbours() const;
    QVector< QVector< int > > GetSerializedPolygonNeighbours() const;
    float GetSilhouetteLength(int pViewpoint) const;
    QVector< float > GetSilhouetteCurvature(int pViewpoint) const;
    QVector< float > GetNormalizedDepthHistogram(int pViewpoint) const;
    cv::Mat GetDepthImage(int pViewpoint) const;
    float GetMaximumDepth(int pViewpoint) const;
    QSet< int > GetVisibleVertices(int pViewpoint) const;
    QVector< float > GetSerializedPolygonAreas() const;
    QVector< float > GetSerializedVertexCurvature() const;
    int GetWidthResolution() const;
    float GetAspectRatio() const;

protected:
    /// Save the OpenGL stats
    void SaveOpenGLStats();
    /// Restore the OpenGL stats
    void RestoreOpenGLStats();

    QVector<QVector<int> > mViewpointNeighbours;
    SerializedSceneGeometry* mSerializedScene;
    /// Matrix with the projected areas of the polygons from every viewpoint
    ProjectedAreasMatrix* mProjectedAreasMatrix;
    /// List of lengths of the silhouettes of the models seen from every viewpoint
    QVector<float> mSilhouetteLengths;
    QVector< QVector<float> > mSilhouetteCurvature;
    QVector< QVector<float> > mNormalizedDepthHistograms;
    QVector<float> mMaxDepths;
    QVector<cv::Mat> mDepthImages;
    QVector< QSet<int> > mVisibleVertexs;
    int mWidthResolution;
    float mAspectRatio;
    /// Program used to paint a different color per face
    GLSLProgram* mShaderColorPerFace;
    /// Shader to do the fixed vertex shader pipeline
    GLSLShader* mBasicVS;
    /// Shader used to paint a different color per face
    GLSLShader* mColorPerFaceFS;
    /// Clear color that was present when the OpenGL stats have been saved
    glm::vec4 mPreviousClearColor;
    /// Viewport that was present when the OpenGL stats have been saved
    GLint mPreviousViewport[4];
    /// Boolean to know if the depth test was activated when the OpenGL stats have been saved
    bool mPreviousDepthTest;
    /// Boolean to know if the cull face was activated when the OpenGL stats have been saved
    bool mPreviousCullFace;
    /// Boolean to know if the blending was activated when the OpenGL stats have been saved
    bool mPreviousBlend;
};

#endif
