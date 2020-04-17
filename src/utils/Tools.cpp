//Definition include
#include "Tools.h"

//Qt includes
#include <QtCore/QDir>

//Dependency includes
#include "glm/common.hpp"
#include "glm/geometric.hpp"

//Project includes
#include "Debug.h"

#include <algorithm>

bool pairCompareX(std::pair<int, glm::vec3> pI, std::pair<int, glm::vec3> pJ)
{
    return (pI.second.x < pJ.second.x);
}

bool pairCompareY(std::pair<int, glm::vec3> pI, std::pair<int, glm::vec3> pJ)
{
    return (pI.second.y < pJ.second.y);
}

bool pairCompareZ(std::pair<int, glm::vec3> pI, std::pair<int, glm::vec3> pJ)
{
    return (pI.second.z < pJ.second.z);
}

std::vector<int> Tools::GetOrderedIndexesByDimension(std::vector<std::pair<int, glm::vec3>> &pValues, int pDimension)
{
    switch (pDimension)
    {
    case 0:
        std::sort(pValues.begin(), pValues.end(), pairCompareX);
        break;
    case 1:
        std::sort(pValues.begin(), pValues.end(), pairCompareY);
        break;
    case 2:
        std::sort(pValues.begin(), pValues.end(), pairCompareZ);
        break;
    }

    std::vector<int> result;
    std::transform(pValues.cbegin(), pValues.cend(), std::back_inserter(result), [](std::pair<int, glm::vec3> pair) -> int { return pair.first; });

    return result;
}

template <class T>
bool pairCompare(std::pair<int, T> i, std::pair<int, T> j)
{
    return (i.second < j.second);
}

std::vector<int> Tools::GetOrderedIndexes(const std::vector<float> &pValues)
{
    int size = pValues.size();

    std::vector<std::pair<int, float>> toSort(size);
    for (int i = 0; i < size; i++)
    {
        toSort[i] = std::pair<int, float>(i, pValues.at(i));
    }

    std::sort(toSort.begin(), toSort.end(), pairCompare<float>);

    std::vector<int> result(size);
    for (int i = 0; i < size; i++)
    {
        result[i] = toSort.at(i).first;
    }

    return result;
}

std::vector<int> Tools::GetPositions(const std::vector<int> &pValues)
{
    int size = pValues.size();

    std::vector<std::pair<int, int>> toSort(size);
    for (int i = 0; i < size; i++)
    {
        toSort[i] = std::pair<int, int>(i, pValues.at(i));
    }

    std::sort(toSort.begin(), toSort.end(), pairCompare<int>);

    std::vector<int> result(size);
    for (int i = 0; i < size; i++)
    {
        result[i] = toSort.at(i).first;
    }

    return result;
}

std::vector<glm::vec4> Tools::ConvertFloatsToColors(const std::vector<float> &pValues, bool pInverted)
{
    return ConvertNormalizedFloatsToColors(ScaleValues(pValues, 0.0f, 1.0f), pInverted);
}

std::vector<float> Tools::ScaleValues(const std::vector<float> &pValues, float pLowerBound, float pUpperBound, float pPercentOfClipping)
{
    float min, max;

    int size = pValues.size();
    if (pPercentOfClipping == 0.0f)
    {
        const auto minmax = std::minmax_element(pValues.cbegin(), pValues.cend());
        min = *minmax.first;
        max = *minmax.second;
    }
    else
    {
        const int offset = glm::round(size * (pPercentOfClipping / 200.0f));
        std::vector<float> orderedValues = pValues;
        std::sort(orderedValues.begin(), orderedValues.end());
        min = orderedValues.at(offset);
        max = orderedValues.at(size - 1 - offset);
    }
    const bool all_values_equal = max == min;
    const float scale = (pUpperBound - pLowerBound) / (max - min);
    std::vector<float> results{pValues};
    if (all_values_equal)
    {
        auto scale_function = [=](float &value) {
            value = glm::clamp(value, pLowerBound, pUpperBound);
        };
        std::for_each(results.begin(), results.end(), scale_function);
    }
    else
    {
        auto scale_function = [=](float &value) {
            value = (glm::clamp(value, min, max) - min) * scale + pLowerBound;
        };
        std::for_each(results.begin(), results.end(), scale_function);
    }

    return results;
}

float Tools::Mean(const std::vector<float> &pValues, const std::vector<float> &pWeights)
{
    float value = 0.0f;
    if (pValues.size() == pWeights.size())
    {
        float sumWeights = 0.0f;
        for (int i = 0; i < pValues.size(); i++)
        {
            value += pValues.at(i) * pWeights.at(i);
            sumWeights += pWeights.at(i);
        }
        value /= sumWeights;
    }
    else
    {
        for (int i = 0; i < pValues.size(); i++)
        {
            value += pValues.at(i);
        }
        value /= pValues.size();
    }
    return value;
}

std::vector<int> Tools::FindNearestThanEpsilonByDimension(int pPosition, const std::vector<std::pair<int, glm::vec3>> &pVector, float pEpsilon, int pDimension)
{
    std::vector<int> result;

    bool nextUp = true;
    bool nextDown = true;
    int i = 1;
    while (nextUp || nextDown)
    {
        if (nextUp)
        {
            if ((pPosition + i) < pVector.size())
            {
                if (glm::abs(pVector.at(pPosition + i).second[pDimension] - pVector.at(pPosition).second[pDimension]) < pEpsilon)
                {
                    result.push_back(pVector.at(pPosition + i).first);
                }
                else
                {
                    nextUp = false;
                }
            }
            else
            {
                nextUp = false;
            }
        }
        if (nextDown)
        {
            if ((pPosition - i) >= 0)
            {
                if (glm::abs(pVector.at(pPosition - i).second[pDimension] - pVector.at(pPosition).second[pDimension]) < pEpsilon)
                {
                    result.push_back(pVector.at(pPosition - i).first);
                }
                else
                {
                    nextDown = false;
                }
            }
            else
            {
                nextDown = false;
            }
        }
        i++;
    }
    return result;
}

std::vector<int> Tools::MergeNeighbours(const std::vector<int> &pVector1, const std::vector<int> &pVector2, const std::vector<int> &pVector3)
{
    int consecutiveElements, previousValue;
    std::vector<int> result;

    int sizeVector1 = pVector1.size();
    int sizeVector2 = pVector2.size();
    int sizeVector3 = pVector3.size();
    std::vector<int> mixedVector(sizeVector1 + sizeVector2 + sizeVector3);
    int offset = 0;
    for (int i = 0; i < sizeVector1; i++)
    {
        mixedVector[offset + i] = pVector1.at(i);
    }
    offset += sizeVector1;
    for (int i = 0; i < sizeVector2; i++)
    {
        mixedVector[offset + i] = pVector2.at(i);
    }
    offset += sizeVector2;
    for (int i = 0; i < sizeVector3; i++)
    {
        mixedVector[offset + i] = pVector3.at(i);
    }
    if (mixedVector.size() > 0)
    {
        qSort(mixedVector.begin(), mixedVector.end());
        previousValue = mixedVector.at(0);
        consecutiveElements = 1;
    }
    for (int i = 1; i < mixedVector.size(); i++)
    {
        int currentValue = mixedVector.at(i);
        if (currentValue == previousValue)
        {
            consecutiveElements++;
            if (consecutiveElements == 3)
            {
                result.push_back(currentValue / 3);
            }
        }
        else
        {
            consecutiveElements = 1;
        }
        previousValue = currentValue;
    }
    return result;
}

std::vector<glm::vec4> Tools::ConvertNormalizedFloatsToColors(const std::vector<float> &pValues, bool pInverted)
{
    int size = pValues.size();

    std::vector<glm::vec4> results(size);
    for (int i = 0; i < size; i++)
    {
        results[i] = ConvertNormalizedFloatToColor(pValues.at(i), pInverted);
    }

    return results;
}

glm::vec4 Tools::ConvertNormalizedFloatToColor(float pValue, bool pInverted)
{
    float percent;

    if (pInverted)
    {
        percent = 1.0f - pValue;
    }
    else
    {
        percent = pValue;
    }
    float value = percent;
    if (percent < 0.25f)
    {
        percent = percent * 4; // / 0.25;
        return glm::vec4(0.0, percent, 1.0, value);
    }
    else if (percent < 0.5f)
    {
        percent = (percent - 0.25f) * 4; // / 0.25;
        return glm::vec4(0.0, 1.0, 1.0 - percent, value);
    }
    else if (percent < 0.75f)
    {
        percent = (percent - 0.5f) * 4; // / 0.25;
        return glm::vec4(percent, 1.0, 0.0, value);
    }
    else
    {
        percent = (percent - 0.75f) * 4; // / 0.25;
        return glm::vec4(1.0, 1.0 - percent, 0.0, value);
    }
}

float Tools::TriangleArea(const glm::vec3 &pA, const glm::vec3 &pB, const glm::vec3 &pC)
{
    glm::vec3 v = glm::cross(pB - pA, pC - pA);

    return glm::length(v) / 2.0f;
}

QString Tools::GetProgramPath()
{
    QString picturesPath, completePath;

    QString programFolder = QString("Quoniam");
#if defined(Q_OS_WIN32)
    picturesPath = QString(qgetenv("USERPROFILE")) + QString("\\Pictures");
    completePath = picturesPath + "\\" + programFolder + "\\";
#elif defined(Q_OS_MAC)
    picturesPath = QString(qgetenv("HOME")) + QString("/Pictures");
    completePath = picturesPath + "/" + programFolder + "/";
#else
    Debug::Warning("I'm on a unidentified operating system!");
#endif
    if (!QDir(completePath).exists())
    {
        if (!QDir(picturesPath).mkdir(programFolder))
        {
            Debug::Warning(QString("Impossible to create the %1 folder!").arg(programFolder));
        }
    }
    return completePath;
}
