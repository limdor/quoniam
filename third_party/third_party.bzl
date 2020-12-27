"""
Load all third party dependencies needed for this project
"""
load("//third_party/catch2:direct.bzl", "load_catch2")
load("//third_party/glm:direct.bzl", "load_glm")
load("//third_party/rules_pkg:direct.bzl", "load_rules_pkg")
load("//third_party/rules_qt:direct.bzl", "load_rules_qt")

def load_third_party_libraries():
    load_catch2()
    load_glm()
    load_rules_pkg()
    load_rules_qt()
