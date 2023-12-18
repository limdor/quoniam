"""
rules_pkg: Bazel rules for packaging and fetching (for Debian and other distribution channels).
"""

load("@bazel_tools//tools/build_defs/repo:utils.bzl", "maybe")
load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

_VERSION = "0.9.1"

def load_rules_pkg():
    maybe(
        http_archive,
        name = "rules_pkg",
        urls = [
            "https://mirror.bazel.build/github.com/bazelbuild/rules_pkg/releases/download/{version}/rules_pkg-{version}.tar.gz".format(version = _VERSION),
            "https://github.com/bazelbuild/rules_pkg/releases/download/{version}/rules_pkg-{version}.tar.gz".format(version = _VERSION),
        ],
        sha256 = "8f9ee2dc10c1ae514ee599a8b42ed99fa262b757058f65ad3c384289ff70c4b8",
    )
