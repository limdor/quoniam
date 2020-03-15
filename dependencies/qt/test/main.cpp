#define CATCH_CONFIG_MAIN

#include <catch2/catch.hpp>
#include <QtCore/QVector>

TEST_CASE( "vector", "qt vector smoke test" ) {
    QVector<int> vector{2,2};
    REQUIRE(vector.at(0) == vector.at(1));
}
