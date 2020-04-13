/// \file SerializedSceneGeometry.h
/// \class SerializedSceneGeometry
/// \author Xavier Bonaventura
/// \author Nicolau Sunyer
/// \author Copyright: (c) Universitat de Girona

#ifndef SERIALIZED_SCENE_GEOMETRY_H
#define SERIALIZED_SCENE_GEOMETRY_H

#include "Scene.h"

#include "glm/vec3.hpp"

class SerializedSceneGeometry
{
public:
    /// Constructor
    explicit SerializedSceneGeometry(std::shared_ptr<Scene const> pScene);
    /// Destructor
    ~SerializedSceneGeometry();

    /// Show neighbours
    void ShowNeighbours() const;
    /// Get vertices
    QVector<glm::vec3> GetVertices() const;
    /// Get vertices curvature
    QVector< float > GetVerticesCurvature() const;
    /// Get face neighbours
    QVector< QVector< int > > GetFacesNeighbours() const;
    /// Get face areas
    QVector< float > GetFacesAreas() const;

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
    void SerializeSceneNodes(const SceneNode* pSceneNode);

    /// List of vertices
    QVector<glm::vec3> mVertexs;
    /// Number of vertices
    int mNumberOfVertexs;
    /// For every vertex, the list of neighbourhood faces
    QVector< QVector<int> > mVertexNeighbors;
    /// List with the curvature of every vertex
    QVector<float> mVertexCurvatures;
    /// List of faces: every face is compound by 3 indexs of the list of vertices
    QVector<glm::ivec3> mFaces;
    /// Number of faces
    int mNumberOfFaces;
    /// For every face, the list of neighbourhood faces
    QVector< QVector<int> > mFaceNeighbors;
    /// List with the area of every face
    QVector< float > mFaceAreas;
    /// Bounding sphere of the scene
    BoundingSphere* mBoundingSphere;
};
#endif
