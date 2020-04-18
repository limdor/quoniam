#ifndef TOOLS_H
#define TOOLS_H

//Qt includes
#include <QtCore/QString>

//Dependency includes
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"

#include <vector>

class Tools
{
public:
    template<typename T>
    static std::vector< T > GetOrderedIndexesByDimension(std::vector<std::pair<T, glm::vec3> > &pValues, int pDimension);
    template<typename T>
    static std::vector< size_t > GetOrderedIndexes(const std::vector< T >& pValues);
    static std::vector< glm::vec4 > ConvertFloatsToColors(const std::vector< float >& pValues, bool pInverted);
    static std::vector< glm::vec4 > ConvertNormalizedFloatsToColors(const std::vector< float >& pValues, bool pInverted);
    static std::vector< float > ScaleValues(const std::vector< float >& pValues, float pLowerBound, float pUpperBound, float pPercentOfClipping = 0.0f);
    static float Mean(const std::vector< float >& pValues, const std::vector<float>& pWeights = std::vector<float>());
    template<typename T>
    static std::vector<T> FindNearestThanEpsilonByDimension(size_t pPosition, const std::vector<std::pair<T, glm::vec3> > &pVector, float pEpsilon, int pDimension);
    template<typename T>
    static std::vector<T> MergeNeighbours(const std::vector<T> &pVector1, const std::vector<T> &pVector2, const std::vector<T> &pVector3);

    static float TriangleArea(const glm::vec3& pA, const glm::vec3& pB, const glm::vec3& pC);

    static QString GetProgramPath();
private:    
    static glm::vec4 ConvertNormalizedFloatToColor(float pValue, bool pInverted);
};

#endif
