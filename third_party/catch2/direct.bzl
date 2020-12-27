"""
Catch2: A modern, C++-native, header-only, test framework for unit-tests, TDD and
BDD - using C++11, C++14, C++17 and later (or C++03 on the Catch1.x branch)
"""

load("@bazel_tools//tools/build_defs/repo:utils.bzl", "maybe")
load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

def load_catch2():
    maybe(
        http_archive,
        name = "catch2",
        url = "https://github.com/catchorg/Catch2/archive/v2.4.2.zip",
        sha256 = "4f4044d4754b5821d8ae90ef47d7558306ea8f1dcd01d01b22fe0f605b9b2f8b",
        strip_prefix = "Catch2-2.4.2",
        build_file = "//third_party/catch2:catch2.BUILD",
    )
