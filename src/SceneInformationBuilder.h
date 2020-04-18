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

#include <memory>
#include <set>
#include <vector>

/// Static class to create an InformationChannelHistogram
class SceneInformationBuilder
{
public:
    /// Create an InformationChannelHistogram given the Scene and the SphereOfViewpoints
    void CreateHistogram(std::shared_ptr<Scene> pScene, std::shared_ptr<SphereOfViewpoints> pSphereOfViewpoints, int pWidthResolution, bool pFaceCulling, bool pIgnoreNormals = false);
    /// Get the histogram
    std::shared_ptr<ProjectedAreasMatrix const> GetProjectedAreasMatrix() const;
    std::vector< std::vector< size_t > > GetViewpointNeighbours() const;
    std::vector< std::vector< size_t > > GetSerializedPolygonNeighbours() const;
    float GetSilhouetteLength(size_t pViewpoint) const;
    std::vector< float > GetSilhouetteCurvature(size_t pViewpoint) const;
    std::vector< float > GetNormalizedDepthHistogram(size_t pViewpoint) const;
    cv::Mat GetDepthImage(size_t pViewpoint) const;
    float GetMaximumDepth(size_t pViewpoint) const;
    std::set< int > GetVisibleVertices(size_t pViewpoint) const;
    std::vector< float > GetSerializedPolygonAreas() const;
    std::vector< float > GetSerializedVertexCurvature() const;
    int GetWidthResolution() const;
    float GetAspectRatio() const;

protected:
    /// Save the OpenGL stats
    void SaveOpenGLStats();
    /// Restore the OpenGL stats
    void RestoreOpenGLStats();

    std::vector<std::vector<size_t> > mViewpointNeighbours;
    std::unique_ptr<SerializedSceneGeometry> mSerializedScene{nullptr};
    /// Matrix with the projected areas of the polygons from every viewpoint
    std::shared_ptr<ProjectedAreasMatrix> mProjectedAreasMatrix{nullptr};
    /// List of lengths of the silhouettes of the models seen from every viewpoint
    std::vector<float> mSilhouetteLengths;
    std::vector< std::vector<float> > mSilhouetteCurvature;
    std::vector< std::vector<float> > mNormalizedDepthHistograms;
    std::vector<float> mMaxDepths;
    std::vector<cv::Mat> mDepthImages;
    std::vector< std::set<int> > mVisibleVertexs;
    int mWidthResolution{640};
    float mAspectRatio{1.0f};
    /// Program used to paint a different color per face
    std::unique_ptr<GLSLProgram> mShaderColorPerFace = nullptr;
    /// Clear color that was present when the OpenGL stats have been saved
    glm::vec4 mPreviousClearColor;
    /// Viewport that was present when the OpenGL stats have been saved
    GLint mPreviousViewport[4];
    /// Boolean to know if the depth test was activated when the OpenGL stats have been saved
    bool mPreviousDepthTest{true};
    /// Boolean to know if the cull face was activated when the OpenGL stats have been saved
    bool mPreviousCullFace{true};
    /// Boolean to know if the blending was activated when the OpenGL stats have been saved
    bool mPreviousBlend{false};
};

#endif
