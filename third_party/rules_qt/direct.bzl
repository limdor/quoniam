"""
rules_pkg: Bazel rules and BUILD targets to make it easy to use Qt from C++ projects built with bazel.
"""

load("@bazel_tools//tools/build_defs/repo:utils.bzl", "maybe")
load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

def load_rules_qt():
    maybe(
        http_archive,
        name = "com_justbuchanan_rules_qt",
        sha256 = "45255953b2a66119cb0497f2fe70343f32c5cabc9fdd3af7d8caf796aeee1690",
        strip_prefix = "bazel_rules_qt-dfd8ea524d6dd28881c9f2ae0acc8637b28d4eed",
        url = "https://github.com/justbuchanan/bazel_rules_qt/archive/dfd8ea524d6dd28881c9f2ae0acc8637b28d4eed.zip",
    )
