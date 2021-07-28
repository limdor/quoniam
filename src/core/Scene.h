#ifndef SCENE_H
#define SCENE_H

#include "BoundingSphere.h"
#include "SceneNode.h"

#include <stack>
#include <cassert>
#include <iterator>
#include <memory>
#include <string>

class Scene
{
public:
    class const_iterator
    {
        std::shared_ptr<SceneNode const> mCurrentNode = nullptr;
        std::stack<int> mChildIndexFromParent;

    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = const SceneNode;
        using difference_type = std::ptrdiff_t;
        using pointer = value_type*;
        using reference = value_type&;

        const_iterator(std::shared_ptr<SceneNode const> currentNode = nullptr):
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
    Scene(const std::string &pName, std::shared_ptr<SceneNode> pSceneRoot, const std::vector<std::shared_ptr<Material>>& pMaterials, const std::vector<std::shared_ptr<Geometry>>& pGeometries, const std::vector<std::shared_ptr<Mesh>>& pMeshes );
    Scene(const Scene& pScene) = delete;
    ~Scene() = default;

    inline const_iterator cbegin() const{return const_iterator{mRootNode};}
    inline const_iterator cend() const{return const_iterator{nullptr};}
    std::string GetName() const;
    std::shared_ptr<SceneNode const> GetRootNode() const;
    std::shared_ptr<BoundingSphere const> GetBoundingSphere() const;
    size_t GetNumberOfPolygons() const;
    size_t GetNumberOfVertices() const;
    void ShowInformation() const;

private:
    std::string mName;
    std::shared_ptr<SceneNode> mRootNode;
    /// List of materials used
    std::vector<std::shared_ptr<Material>> mMaterials;
    /// List of geometries used
    std::vector<std::shared_ptr<Geometry>> mGeometries;
    /// List of meshes used
    std::vector<std::shared_ptr<Mesh>> mMeshes;
};

#endif
