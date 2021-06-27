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
        url = "https://github.com/catchorg/Catch2/archive/refs/tags/v2.13.6.zip",
        sha256 = "39d50f5d1819cdf2908066664d57c2cde4a4000c364ad3376ea099735c896ff4",
        strip_prefix = "Catch2-2.13.6",
        build_file = "//third_party/catch2:catch2.BUILD",
    )
