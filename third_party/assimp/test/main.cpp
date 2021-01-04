#define CATCH_CONFIG_MAIN

#include <catch2/catch.hpp>
#include "assimp/Importer.hpp"

TEST_CASE( "3ds", "Supported formats" ) {
    Assimp::Importer importer;
    REQUIRE(importer.IsExtensionSupported(".3ds"));
}

TEST_CASE( "obj", "Supported formats" ) {
    Assimp::Importer importer;
    REQUIRE(importer.IsExtensionSupported(".obj"));
}
