/// \file Scene.h
/// \class Scene
/// \author Xavier Bonaventura
/// \author Nicolau Sunyer
/// \author Copyright: (c) Universitat de Girona

#ifndef SCENE_H
#define SCENE_H

#include "BoundingSphere.h"
#include "SceneNode.h"

#include <QtCore/QString>

#include <stack>
#include <cassert>
#include <iterator>
#include <memory>

class Scene
{
public:
    class const_iterator: std::iterator<std::forward_iterator_tag, const SceneNode>
    {
        const SceneNode* mCurrentNode = nullptr;
        std::stack<int> mChildIndexFromParent;

    public:
        const_iterator(const SceneNode* currentNode = nullptr):
            mCurrentNode{currentNode}, mChildIndexFromParent{} {}
        const_iterator& operator++() {
            if(mCurrentNode) {
                if(mCurrentNode->GetNumChilds() > 0) {
                    mCurrentNode = mCurrentNode->GetChild(0);
                    mChildIndexFromParent.push(0);
                }
                else{
                    bool nextFound = false;
                    while(!nextFound && mCurrentNode->GetParent() != nullptr && !mChildIndexFromParent.empty())
                    {
                        mCurrentNode = mCurrentNode->GetParent();
                        int nextChild = mChildIndexFromParent.top() + 1;
                        mChildIndexFromParent.pop();
                        if(nextChild < mCurrentNode->GetNumChilds())
                        {
                            mCurrentNode = mCurrentNode->GetChild(nextChild);
                            mChildIndexFromParent.push(nextChild);
                            nextFound = true;
                        }
                    }
                    if(!nextFound)
                    {
                        mCurrentNode = nullptr;
                        assert(mChildIndexFromParent.empty());
                    }
                }
            }
            return *this;
            }
        bool operator==(const_iterator other) const {return mCurrentNode == other.mCurrentNode;}
        bool operator!=(const_iterator other) const {return !(*this == other);}
        reference operator*() const {return *mCurrentNode;}
    };
    /// Create an scene given the name and the root scene node
    Scene(const QString &pName, SceneNode *pSceneRoot, const QVector<std::shared_ptr<Material>>& pMaterials, const QVector<std::shared_ptr<Geometry>>& pGeometries, const QVector<std::shared_ptr<Mesh>>& pMeshes );
    Scene(const Scene& pScene) = delete;
    ~Scene();

    inline const_iterator cbegin() const{return const_iterator{mRootNode};}
    inline const_iterator cend() const{return const_iterator{nullptr};}
    QString GetName() const;
    const SceneNode* GetRootNode() const;
    std::shared_ptr<BoundingSphere const> GetBoundingSphere() const;
    int GetNumberOfPolygons() const;
    int GetNumberOfVertices() const;
    void ShowInformation() const;

private:
    QString mName;
    SceneNode* mRootNode;
    /// List of materials used
    QVector<std::shared_ptr<Material>> mMaterials;
    /// List of geometries used
    QVector<std::shared_ptr<Geometry>> mGeometries;
    /// List of meshes used
    QVector<std::shared_ptr<Mesh>> mMeshes;
};

#endif
