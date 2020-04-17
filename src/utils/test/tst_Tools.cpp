#define CATCH_CONFIG_MAIN

#include <catch2/catch.hpp>
#include "Tools.h"

namespace Catch
{
template <>
struct StringMaker<glm::vec4>
{
    static std::string convert(glm::vec4 const &value)
    {
        return "(" + std::to_string(value.r) + ", " +
               std::to_string(value.g) + ", " +
               std::to_string(value.b) + ", " +
               std::to_string(value.a) + ")";
    }
};
} // namespace Catch

TEST_CASE("TriangleArea", "Tools")
{
    const float area = Tools::TriangleArea(glm::vec3{0.0f, 0.0f, 0.0f}, glm::vec3{0.0f, 0.0f, 0.0f}, glm::vec3{0.0f, 0.0f, 0.0f});
    REQUIRE(area == Approx(0.0f));
}

TEST_CASE("GetOrderedIndexesByDimension", "Tools")
{
    std::vector<std::pair<int, glm::vec3>> input{
        {0, glm::vec3{1.0f, 2.0f, 3.0f}},
        {1, glm::vec3{2.0f, 3.0f, 1.0f}},
        {2, glm::vec3{3.0f, 1.0f, 2.0f}},
    };

    const std::vector<int> result_x = Tools::GetOrderedIndexesByDimension(input, 0);
    const std::vector<int> expected_indexes_x{0, 1, 2};
    const std::vector<std::pair<int, glm::vec3>> expected_sorted_x{
        {0, glm::vec3{1.0f, 2.0f, 3.0f}},
        {1, glm::vec3{2.0f, 3.0f, 1.0f}},
        {2, glm::vec3{3.0f, 1.0f, 2.0f}},
    };
    REQUIRE(result_x == expected_indexes_x);
    REQUIRE(input == expected_sorted_x);

    const std::vector<int> result_y = Tools::GetOrderedIndexesByDimension(input, 1);
    const std::vector<int> expected_indexes_y{2, 0, 1};
    const std::vector<std::pair<int, glm::vec3>> expected_sorted_y{
        {2, glm::vec3{3.0f, 1.0f, 2.0f}},
        {0, glm::vec3{1.0f, 2.0f, 3.0f}},
        {1, glm::vec3{2.0f, 3.0f, 1.0f}},
    };
    REQUIRE(result_y == expected_indexes_y);
    REQUIRE(input == expected_sorted_y);

    const std::vector<int> result_z = Tools::GetOrderedIndexesByDimension(input, 2);
    const std::vector<int> expected_indexes_z{1, 2, 0};
    const std::vector<std::pair<int, glm::vec3>> expected_sorted_z{
        {1, glm::vec3{2.0f, 3.0f, 1.0f}},
        {2, glm::vec3{3.0f, 1.0f, 2.0f}},
        {0, glm::vec3{1.0f, 2.0f, 3.0f}},
    };
    REQUIRE(result_z == expected_indexes_z);
    REQUIRE(input == expected_sorted_z);
}

TEST_CASE("GetOrderedIndexes", "Tools")
{
    const std::vector<float> input{2.0f, 4.0f, 1.0f, 3.0f};

    const std::vector<int> result = Tools::GetOrderedIndexes(input);
    const std::vector<int> expected{2, 0, 3, 1};
    REQUIRE(result == expected);
}

TEST_CASE("GetPositions", "Tools")
{
    const std::vector<int> input{2, 4, 1, 3};

    const std::vector<int> result = Tools::GetPositions(input);
    const std::vector<int> expected{2, 0, 3, 1};
    REQUIRE(result == expected);
}

SCENARIO("colors can be converted to colors", "ConvertFloatsToColors")
{
    const glm::vec3 GREEN{0.0f, 1.0f, 0.0f};
    const glm::vec3 RED{1.0f, 0.0f, 0.0f};
    const glm::vec3 BLUE{0.0f, 0.0f, 1.0f};
    GIVEN("A vector with three unsorted values")
    {
        const QVector<float> input{1.0f, 0.0f, 2.0f};

        WHEN("converted without inversion")
        {
            const QVector<glm::vec4> result = Tools::ConvertFloatsToColors(input, false);

            THEN("lower gets blue, middle gets green and higher gets red")
            {
                const QVector<glm::vec4> expected{
                    glm::vec4{GREEN, 0.5f},
                    glm::vec4{BLUE, 0.0f},
                    glm::vec4{RED, 1.0f}};
                REQUIRE(result == expected);
            }
        }

        WHEN("converted with inversion")
        {
            const QVector<glm::vec4> result = Tools::ConvertFloatsToColors(input, true);

            THEN("lower gets red, middle gets green and higher gets blue")
            {
                const QVector<glm::vec4> expected{
                    glm::vec4{GREEN, 0.5f},
                    glm::vec4{RED, 1.0f},
                    glm::vec4{BLUE, 0.0f}};
                REQUIRE(result == expected);
            }
        }
    }

    GIVEN("A vector with all values equal")
    {
        const QVector<float> input{2.0f, 2.0f, 2.0f};

        WHEN("converted without inversion")
        {
            const QVector<glm::vec4> result = Tools::ConvertFloatsToColors(input, false);

            THEN("all get green")
            {
                const QVector<glm::vec4> expected{
                    glm::vec4{RED, 1.0f},
                    glm::vec4{RED, 1.0f},
                    glm::vec4{RED, 1.0f}};
                REQUIRE(result == expected);
            }
        }
    }
}

SCENARIO("values can be scaled within a range", "ScaleValues")
{
    const float LOWER_BOUND = 0.0f;
    const float UPPER_BOUND = 2.0f;
    GIVEN("A vector of different values")
    {
        const QVector<float> input{1.0f, 2.0f, 3.0f, 4.0f, 5.0f};

        WHEN("scaled between two different values")
        {
            const QVector<float> result = Tools::ScaleValues(input, LOWER_BOUND, UPPER_BOUND);

            THEN("the lowest value gets the lower bound, the upper value the higher bound and the others the values in between")
            {
                const QVector<float> expected{LOWER_BOUND, 0.5f, 1.0f, 1.5f, UPPER_BOUND};
                REQUIRE(result == expected);
            }
        }
    }

    GIVEN("A vector of same values inside [LOWER_BOUND, UPPER_BOUND]")
    {
        const QVector<float> input{2.0f, 2.0f, 2.0f, 2.0f, 2.0f};

        WHEN("scaled between [LOWER_BOUND, UPPER_BOUND]")
        {
            const QVector<float> result = Tools::ScaleValues(input, LOWER_BOUND, UPPER_BOUND);

            THEN("the output is the same like the input")
            {
                REQUIRE(result == input);
            }
        }
    }

    GIVEN("A vector of same values outside [LOWER_BOUND, UPPER_BOUND]")
    {
        const QVector<float> input{4.0f, 4.0f, 4.0f, 4.0f, 4.0f};

        WHEN("scaled between [LOWER_BOUND, UPPER_BOUND]")
        {
            const QVector<float> result = Tools::ScaleValues(input, LOWER_BOUND, UPPER_BOUND);

            THEN("the output is all values the same like the closer bound")
            {
                const QVector<float> expected{UPPER_BOUND, UPPER_BOUND, UPPER_BOUND, UPPER_BOUND, UPPER_BOUND};
                REQUIRE(result == expected);
            }
        }
    }
}
