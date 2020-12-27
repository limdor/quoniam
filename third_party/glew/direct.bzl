"""
glew: The OpenGL Extension Wrangler Library is a simple tool that helps C/C++ developers
initialize extensions and write portable applications.
"""

load("@bazel_tools//tools/build_defs/repo:utils.bzl", "maybe")
load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

def load_glew():
    maybe(
        http_archive,
        name = "glew",
        url = "https://iweb.dl.sourceforge.net/project/glew/glew/1.12.0/glew-1.12.0-win32.zip",
        sha256 = "b222ce3622661dd5c2ecef6e8f3832db7a2d6e3e46723743a22befd88ff37a7a",
        strip_prefix = "glew-1.12.0",
        build_file = "//third_party/glew:glew.BUILD",
    )
