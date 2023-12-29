#define CATCH_CONFIG_MAIN

#include <catch2/catch_test_macros.hpp>
#include "glew.h"

TEST_CASE( "fail without valid OpenGL context", "glew init" ) {
    GLenum err = glewInit();
    REQUIRE(GLEW_OK != err);
}
