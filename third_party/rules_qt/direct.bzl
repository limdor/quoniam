"""
rules_pkg: Bazel rules and BUILD targets to make it easy to use Qt from C++ projects built with bazel.
"""

load("@bazel_tools//tools/build_defs/repo:utils.bzl", "maybe")
load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

def load_rules_qt():
    maybe(
        http_archive,
        name = "com_limdor_rules_qt",
        sha256 = "94e7b9c77620433185688a418c87efc18524fcd3a22b83d520942fd62e6983d6",
        strip_prefix = "bazel_rules_qt-0.1.0",
        url = "https://github.com/limdor/bazel_rules_qt/archive/v0.1.0.tar.gz",
    )
