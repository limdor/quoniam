#define CATCH_CONFIG_MAIN

#include <catch2/catch.hpp>
#include <glm/vec3.hpp>
#include <glm/gtc/epsilon.hpp>

TEST_CASE( "sum works", "glm basic types" ) {
    const glm::vec3 point1{1.0f, 1.0f, 1.0f};
    const glm::vec3 point2{2.0f, 2.0f, 2.0f};
    const glm::vec3 result = point1 + point2;
    REQUIRE(glm::all(glm::epsilonEqual(result, glm::vec3{3.0f, 3.0f, 3.0f}, 0.01f)));
}
