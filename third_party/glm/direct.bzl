"""
glm: OpenGL Mathematics (GLM) is a header only C++ mathematics library for graphics software based
on the OpenGL Shading Language (GLSL) specifications.

http://glm.g-truc.net/download.html
"""

load("@bazel_tools//tools/build_defs/repo:utils.bzl", "maybe")
load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

def load_glm():
    maybe(
        http_archive,
        name = "glm",
        url = "https://github.com/g-truc/glm/releases/download/0.9.9.8/glm-0.9.9.8.zip",
        sha256 = "37e2a3d62ea3322e43593c34bae29f57e3e251ea89f4067506c94043769ade4c",
        strip_prefix = "glm",
        build_file = "//third_party/glm:glm.BUILD",
    )
