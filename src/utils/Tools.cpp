//Definition include
#include "Tools.h"

//Qt includes
#include <QtCore/QtGlobal>

//Dependency includes
#include "glm/common.hpp"
#include "glm/geometric.hpp"

//Project includes
#include "Debug.h"

#include <algorithm>
#include <cstdlib>
#include <filesystem>

template<typename T>
bool pairCompareX(std::pair<T, glm::vec3> pI, std::pair<T, glm::vec3> pJ)
{
    return (pI.second.x < pJ.second.x);
}

template<typename T>
bool pairCompareY(std::pair<T, glm::vec3> pI, std::pair<T, glm::vec3> pJ)
{
    return (pI.second.y < pJ.second.y);
}

template<typename T>
bool pairCompareZ(std::pair<T, glm::vec3> pI, std::pair<T, glm::vec3> pJ)
{
    return (pI.second.z < pJ.second.z);
}

template<typename T>
std::vector<T> Tools::GetOrderedIndexesByDimension(std::vector<std::pair<T, glm::vec3>> &pValues, int pDimension)
{
    switch (pDimension)
    {
    case 0:
        std::sort(pValues.begin(), pValues.end(), pairCompareX<T>);
        break;
    case 1:
        std::sort(pValues.begin(), pValues.end(), pairCompareY<T>);
        break;
    case 2:
        std::sort(pValues.begin(), pValues.end(), pairCompareZ<T>);
        break;
    }

    std::vector<T> result;
    std::transform(pValues.cbegin(), pValues.cend(), std::back_inserter(result), [](std::pair<T, glm::vec3> pair) -> T { return pair.first; });

    return result;
}

template std::vector<int> Tools::GetOrderedIndexesByDimension(std::vector<std::pair<int, glm::vec3>> &pValues, int pDimension);
template std::vector<size_t> Tools::GetOrderedIndexesByDimension(std::vector<std::pair<size_t, glm::vec3>> &pValues, int pDimension);

template <class T>
bool pairCompare(std::pair<size_t, T> i, std::pair<size_t, T> j)
{
    return (i.second < j.second);
}

template <typename T>
std::vector<size_t> Tools::GetOrderedIndexes(const std::vector<T> &pValues)
{
    size_t size = pValues.size();

    std::vector<std::pair<size_t, T>> toSort(size);
    for (size_t i = 0; i < size; i++)
    {
        toSort[i] = std::pair<size_t, T>(i, pValues.at(i));
    }

    std::sort(toSort.begin(), toSort.end(), pairCompare<T>);

    std::vector<size_t> result;
    result.reserve(size);
    std::transform(toSort.cbegin(), toSort.cend(), std::back_inserter(result),
                   [](std::pair<size_t, T> const &pair) -> size_t { return pair.first; });

    return result;
}

template std::vector<size_t> Tools::GetOrderedIndexes(const std::vector<int> &pValues);
template std::vector<size_t> Tools::GetOrderedIndexes(const std::vector<float> &pValues);
template std::vector<size_t> Tools::GetOrderedIndexes(const std::vector<size_t> &pValues);

std::vector<glm::vec4> Tools::ConvertFloatsToColors(const std::vector<float> &pValues, bool pInverted)
{
    return ConvertNormalizedFloatsToColors(ScaleValues(pValues, 0.0f, 1.0f), pInverted);
}

std::vector<float> Tools::ScaleValues(const std::vector<float> &pValues, float pLowerBound, float pUpperBound, float pPercentOfClipping)
{
    float min, max;

    if (pPercentOfClipping == 0.0f)
    {
        const auto minmax = std::minmax_element(pValues.cbegin(), pValues.cend());
        min = *minmax.first;
        max = *minmax.second;
    }
    else
    {
        const size_t size = pValues.size();
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

template <typename T>
std::vector<T> Tools::FindNearestThanEpsilonByDimension(size_t pPosition, const std::vector<std::pair<T, glm::vec3>> &pVector, float pEpsilon, int pDimension)
{
    std::vector<T> result;

    bool nextUp = true;
    bool nextDown = true;
    size_t i = 1;
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
            if (pPosition >= i)
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

template std::vector<int> Tools::FindNearestThanEpsilonByDimension(size_t pPosition, const std::vector<std::pair<int, glm::vec3>> &pVector, float pEpsilon, int pDimension);
template std::vector<size_t> Tools::FindNearestThanEpsilonByDimension(size_t pPosition, const std::vector<std::pair<size_t, glm::vec3>> &pVector, float pEpsilon, int pDimension);

template <typename T>
std::vector<T> Tools::MergeNeighbours(const std::vector<T> &pVector1, const std::vector<T> &pVector2, const std::vector<T> &pVector3)
{
    int consecutiveElements;
    T previousValue;
    std::vector<T> result;

    const size_t sizeVector1 = pVector1.size();
    const size_t sizeVector2 = pVector2.size();
    const size_t sizeVector3 = pVector3.size();
    std::vector<T> mixedVector(sizeVector1 + sizeVector2 + sizeVector3);
    size_t offset = 0;
    for (size_t i = 0; i < sizeVector1; i++)
    {
        mixedVector[offset + i] = pVector1.at(i);
    }
    offset += sizeVector1;
    for (size_t i = 0; i < sizeVector2; i++)
    {
        mixedVector[offset + i] = pVector2.at(i);
    }
    offset += sizeVector2;
    for (size_t i = 0; i < sizeVector3; i++)
    {
        mixedVector[offset + i] = pVector3.at(i);
    }
    if (!mixedVector.empty())
    {
        std::sort(mixedVector.begin(), mixedVector.end());
        previousValue = mixedVector.at(0);
        consecutiveElements = 1;
    }
    for (size_t i = 1; i < mixedVector.size(); i++)
    {
        T currentValue = mixedVector.at(i);
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

template std::vector<int> Tools::MergeNeighbours(const std::vector<int> &pVector1, const std::vector<int> &pVector2, const std::vector<int> &pVector3);
template std::vector<size_t> Tools::MergeNeighbours(const std::vector<size_t> &pVector1, const std::vector<size_t> &pVector2, const std::vector<size_t> &pVector3);

std::vector<glm::vec4> Tools::ConvertNormalizedFloatsToColors(const std::vector<float> &pValues, bool pInverted)
{
    std::vector<glm::vec4> results;
    std::transform(pValues.cbegin(), pValues.cend(), std::back_inserter(results),
     [pInverted](float value) -> glm::vec4 { return ConvertNormalizedFloatToColor(value, pInverted); });

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

std::filesystem::path Tools::GetProgramPath()
{
#if defined(Q_OS_WIN32)
    std::string const home_environment_variable{"USERPROFILE"};
#elif defined(Q_OS_MAC)
    std::string const home_environment_variable{"HOME"};
#else
    std::string const home_environment_variable{"USERPROFILE"};
    Debug::Warning("I'm on a unidentified operating system!");
#endif
    std::filesystem::path const home_path{std::getenv(home_environment_variable.c_str())};
    std::filesystem::path const pictures_path = home_path / "Pictures";
    std::string const program_folder{"Quoniam"}; //TODO: Change to constexpr once supported by the compiler
    std::filesystem::path const complete_path = pictures_path / program_folder;

    if (!std::filesystem::is_directory(complete_path))
    {
        if (!std::filesystem::create_directories(complete_path))
        {
            Debug::Warning("Impossible to create the " + complete_path.string() + " folder!");
        }
    }

    return complete_path;
}
