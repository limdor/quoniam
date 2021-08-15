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
#include <ranges>
#include <utility>

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

template <typename T>
std::vector<std::size_t> Tools::GetOrderedIndexes(const std::vector<T> &pValues)
{
    std::vector<std::pair<size_t, T>> toSort = addIndex(pValues);
    std::ranges::sort(toSort,
                      [](std::pair<size_t, T> const & i, std::pair<size_t, T> const & j) -> bool { return i.second < j.second; });

    auto firstElement =  [](std::pair<size_t, T> const &pair) -> size_t { return pair.first; };
    auto const result_view = toSort | std::views::transform(firstElement);
    std::vector<std::size_t> result(result_view.begin(), result_view.end());

    return result;
}

template std::vector<size_t> Tools::GetOrderedIndexes(const std::vector<int> &pValues);
template std::vector<size_t> Tools::GetOrderedIndexes(const std::vector<float> &pValues);
template std::vector<size_t> Tools::GetOrderedIndexes(const std::vector<size_t> &pValues);

template <typename T>
std::vector< std::pair<std::size_t, T> > Tools::addIndex(const std::vector<T> &values)
{
    auto makePair = [](T const& value, std::size_t index)-> std::pair<std::size_t, T>{ return std::make_pair(index, value); };
    std::vector<std::pair<std::size_t, T>> valuesWithIndex;
    valuesWithIndex.reserve(values.size());
    std::ranges::transform(values, std::views::iota(0), std::back_inserter(valuesWithIndex), makePair);

    return valuesWithIndex;
}

template std::vector<std::pair<std::size_t, int>> Tools::addIndex(const std::vector<int> &values);
template std::vector<std::pair<std::size_t, float>> Tools::addIndex(const std::vector<float> &values);
template std::vector<std::pair<std::size_t, std::size_t>> Tools::addIndex(const std::vector<std::size_t> &values);

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
    const size_t sizeAllInputs = pVector1.size() + pVector2.size() + pVector3.size();
    if(sizeAllInputs == 0){
        return {};
    }

    std::vector<T> mixedVector{};
    mixedVector.reserve(sizeAllInputs);
    std::copy(pVector1.cbegin(), pVector1.cend(), std::back_inserter(mixedVector));
    std::copy(pVector2.cbegin(), pVector2.cend(), std::back_inserter(mixedVector));
    std::copy(pVector3.cbegin(), pVector3.cend(), std::back_inserter(mixedVector));
    
    std::sort(mixedVector.begin(), mixedVector.end());

    T previousValue = mixedVector.at(0);
    int consecutiveElements = 1;
    std::vector<T> result;
    for (size_t i = 1; i < sizeAllInputs; i++)
    {
        T const currentValue = mixedVector.at(i);
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
            previousValue = currentValue;
        }
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
