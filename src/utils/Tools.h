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
    static std::vector< int > GetOrderedIndexesByDimension(std::vector<std::pair<int, glm::vec3> > &pValues, int pDimension);
    static std::vector< int > GetOrderedIndexes(const std::vector< float >& pValues);
    static std::vector< int > GetPositions(const std::vector< int >& pValues);
    static std::vector< glm::vec4 > ConvertFloatsToColors(const std::vector< float >& pValues, bool pInverted);
    static std::vector< glm::vec4 > ConvertNormalizedFloatsToColors(const std::vector< float >& pValues, bool pInverted);
    static std::vector< float > ScaleValues(const std::vector< float >& pValues, float pLowerBound, float pUpperBound, float pPercentOfClipping = 0.0f);
    static float Mean(const std::vector< float >& pValues, const std::vector<float>& pWeights = std::vector<float>());

    static std::vector<int> FindNearestThanEpsilonByDimension(int pPosition, const std::vector<std::pair<int, glm::vec3> > &pVector, float pEpsilon, int pDimension);
    static std::vector<int> MergeNeighbours(const std::vector<int> &pVector1, const std::vector<int> &pVector2, const std::vector<int> &pVector3);

    static float TriangleArea(const glm::vec3& pA, const glm::vec3& pB, const glm::vec3& pC);

    static QString GetProgramPath();
private:    
    static glm::vec4 ConvertNormalizedFloatToColor(float pValue, bool pInverted);
};

#endif
