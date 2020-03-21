#define CATCH_CONFIG_MAIN

#include <catch2/catch.hpp>
#include "Tools.h"

TEST_CASE("TriangleArea", "Tools")
{
    const float area = Tools::TriangleArea(glm::vec3{0.0f, 0.0f, 0.0f}, glm::vec3{0.0f, 0.0f, 0.0f}, glm::vec3{0.0f, 0.0f, 0.0f});
    REQUIRE(area == Approx(0.0f));
}

TEST_CASE("GetOrderedIndexesByDimension", "Tools")
{
    QVector<QPair<int, glm::vec3>> input{
        QPair<int, glm::vec3>(0, glm::vec3{1.0f, 2.0f, 3.0f}),
        QPair<int, glm::vec3>(1, glm::vec3{2.0f, 3.0f, 1.0f}),
        QPair<int, glm::vec3>(2, glm::vec3{3.0f, 1.0f, 2.0f}),
    };

    const QVector<int> result_x = Tools::GetOrderedIndexesByDimension(input, 0);
    const QVector<int> expected_indexes_x{0, 1, 2};
    const QVector<QPair<int, glm::vec3>> expected_sorted_x{
        QPair<int, glm::vec3>(0, glm::vec3{1.0f, 2.0f, 3.0f}),
        QPair<int, glm::vec3>(1, glm::vec3{2.0f, 3.0f, 1.0f}),
        QPair<int, glm::vec3>(2, glm::vec3{3.0f, 1.0f, 2.0f}),
    };
    REQUIRE(result_x == expected_indexes_x);
    REQUIRE(input == expected_sorted_x);

    const QVector<int> result_y = Tools::GetOrderedIndexesByDimension(input, 1);
    const QVector<int> expected_indexes_y{2, 0, 1};
    const QVector<QPair<int, glm::vec3>> expected_sorted_y{
        QPair<int, glm::vec3>(2, glm::vec3{3.0f, 1.0f, 2.0f}),
        QPair<int, glm::vec3>(0, glm::vec3{1.0f, 2.0f, 3.0f}),
        QPair<int, glm::vec3>(1, glm::vec3{2.0f, 3.0f, 1.0f}),
    };
    REQUIRE(result_y == expected_indexes_y);
    REQUIRE(input == expected_sorted_y);

    const QVector<int> result_z = Tools::GetOrderedIndexesByDimension(input, 2);
    const QVector<int> expected_indexes_z{1, 2, 0};
    const QVector<QPair<int, glm::vec3>> expected_sorted_z{
        QPair<int, glm::vec3>(1, glm::vec3{2.0f, 3.0f, 1.0f}),
        QPair<int, glm::vec3>(2, glm::vec3{3.0f, 1.0f, 2.0f}),
        QPair<int, glm::vec3>(0, glm::vec3{1.0f, 2.0f, 3.0f}),
    };
    REQUIRE(result_z == expected_indexes_z);
    REQUIRE(input == expected_sorted_z);
}
