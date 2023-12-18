"""
Load all third party dependencies needed for this project
"""
load("//third_party/catch2:direct.bzl", "load_catch2")
load("//third_party/glew:direct.bzl", "load_glew")
load("//third_party/rules_qt:direct.bzl", "load_rules_qt")

def load_third_party_libraries():
    """Load all third party dependencies"""
    load_catch2()
    load_glew()
    load_rules_qt()
