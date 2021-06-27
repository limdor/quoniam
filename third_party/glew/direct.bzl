"""
glew: The OpenGL Extension Wrangler Library is a simple tool that helps C/C++ developers
initialize extensions and write portable applications.

http://glew.sourceforge.net/
"""

load("@bazel_tools//tools/build_defs/repo:utils.bzl", "maybe")
load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

def load_glew():
    maybe(
        http_archive,
        name = "glew",
        url = "https://github.com/nigels-com/glew/releases/download/glew-2.2.0/glew-2.2.0-win32.zip",
        sha256 = "ea6b14a1c6c968d0034e61ff6cb242cff2ce0ede79267a0f2b47b1b0b652c164",
        strip_prefix = "glew-2.2.0",
        build_file = "//third_party/glew:glew.BUILD",
    )
