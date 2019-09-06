#define CATCH_CONFIG_MAIN

#include <catch2/catch.hpp>
#include "Miniball.h"

TEST_CASE( "basic test", "miniball" ) {
    Miniball<3> mb;
    Point<3> p;
    for( int i = 0; i < 3; i++ )
    {
        p[0] = 1.0f;
        p[1] = 1.0f;
        p[2] = 1.0f;
        mb.check_in(p);
    }
    mb.build();
    float x = mb.center()[0];
    float y = mb.center()[1];
    float z = mb.center()[2];
    REQUIRE(std::abs(x - 1.0f) < 0.01f);
    REQUIRE(std::abs(y - 1.0f) < 0.01f);
    REQUIRE(std::abs(z - 1.0f) < 0.01f);
}
