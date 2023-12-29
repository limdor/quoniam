#define CATCH_CONFIG_MAIN

#include <catch2/catch_test_macros.hpp>
#include "opencv2/core/core.hpp"

TEST_CASE( "core", "opencv smoke test" ) {
    cv::Mat m1{2, 2, CV_8UC3, cv::Scalar{0,0,255}};
    cv::Mat m2 = m1.clone();
    REQUIRE(m1.total() == m2.total());
}
