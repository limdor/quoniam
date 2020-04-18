#ifndef SERIALIZED_SCENE_GEOMETRY_H
#define SERIALIZED_SCENE_GEOMETRY_H

#include "Scene.h"

#include "glm/vec3.hpp"

#include <memory>
#include <vector>

class SerializedSceneGeometry
{
public:
    explicit SerializedSceneGeometry(std::shared_ptr<Scene const> pScene);

    /// Show neighbours
    void ShowNeighbours() const;
    /// Get vertices
    std::vector<glm::vec3> GetVertices() const;
    /// Get vertices curvature
    std::vector< float > GetVerticesCurvature() const;
    /// Get face neighbours
    std::vector< std::vector< size_t > > GetFacesNeighbours() const;
    /// Get face areas
    std::vector< float > GetFacesAreas() const;

private:
    /// Compute the bounding sphere
    void ComputeBoundingSphere();
    /// Compute neighbourhood
    void ComputeNeighbourhood();
    /// Compute vertex curvature
    void ComputeVertexCurvatures();
    /// Compute the area of the faces
    void ComputeFaceAreas();
    /// Serialize the scene
    void SerializeSceneNodes(std::shared_ptr<SceneNode const> pSceneNode);

    /// List of vertices
    std::vector<glm::vec3> mVertexs;
    /// Number of vertices
    int mNumberOfVertexs;
    /// For every vertex, the list of neighbourhood faces
    std::vector< std::vector<size_t> > mVertexNeighbors;
    /// List with the curvature of every vertex
    std::vector<float> mVertexCurvatures;
    /// List of faces: every face is compound by 3 indexs of the list of vertices
    std::vector<glm::ivec3> mFaces;
    /// Number of faces
    size_t mNumberOfFaces;
    /// For every face, the list of neighbourhood faces
    std::vector< std::vector<size_t> > mFaceNeighbors;
    /// List with the area of every face
    std::vector< float > mFaceAreas;
    /// Bounding sphere of the scene
    std::unique_ptr<BoundingSphere> mBoundingSphere;
};
#endif
