#define CATCH_CONFIG_MAIN

#include <catch2/catch.hpp>
#include "Tools.h"

TEST_CASE( "Sort array", "Tools" ) {
    const float area = Tools::TriangleArea(glm::vec3{0.0f, 0.0f, 0.0f}, glm::vec3{0.0f, 0.0f, 0.0f}, glm::vec3{0.0f, 0.0f, 0.0f});
    REQUIRE( area == Approx( 0.0f ) );
}
