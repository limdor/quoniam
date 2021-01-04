"""
Load all third party dependencies needed for this project
"""
load("//third_party/bazel_skylib:direct.bzl", "load_bazel_skylib")
load("//third_party/catch2:direct.bzl", "load_catch2")
load("//third_party/glew:direct.bzl", "load_glew")
load("//third_party/glm:direct.bzl", "load_glm")
load("//third_party/rules_pkg:direct.bzl", "load_rules_pkg")
load("//third_party/rules_python:direct.bzl", "load_rules_python")
load("//third_party/rules_qt:direct.bzl", "load_rules_qt")

def load_third_party_libraries():
    """Load all third party dependencies"""
    load_bazel_skylib()
    load_catch2()
    load_glew()
    load_glm()
    load_rules_pkg()
    load_rules_python()
    load_rules_qt()
