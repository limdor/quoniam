#ifndef TOOLS_H
#define TOOLS_H

//Qt includes
#include <QtCore/QVector>

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
    static QVector< glm::vec4 > ConvertNormalizedFloatsToColors(const QVector< float >& pValues, bool pInverted);
    static std::vector< float > ScaleValues(const std::vector< float >& pValues, float pLowerBound, float pUpperBound, float pPercentOfClipping = 0.0f);
    static float Mean(const QVector< float >& pValues, const QVector<float>& pWeights = QVector<float>());

    static QVector<int> FindNearestThanEpsilonByDimension(int pPosition, const QVector<std::pair<int, glm::vec3> > &pVector, float pEpsilon, int pDimension);
    static QVector<int> MergeNeighbours(const QVector<int> &pVector1, const QVector<int> &pVector2, const QVector<int> &pVector3);

    static float TriangleArea(const glm::vec3& pA, const glm::vec3& pB, const glm::vec3& pC);

    static QString GetProgramPath();
private:    
    static glm::vec4 ConvertNormalizedFloatToColor(float pValue, bool pInverted);
};

#endif
