"""
glm: OpenGL Mathematics (GLM) is a header only C++ mathematics library for graphics software based
on the OpenGL Shading Language (GLSL) specifications.
"""

load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

def load_glm():
    http_archive(
        name = "glm",
        url = "https://github.com/g-truc/glm/archive/0.9.6.1.zip",
        sha256 = "e555da763d92114c00171db9a826b33001f0f0bc44d088097a7066b40e3ba8e3",
        strip_prefix = "glm-0.9.6.1",
        build_file = "//third_party/glm:glm.BUILD",
    )