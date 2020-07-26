"""
Load all third party dependencies needed for this project
"""
load("//third_party/catch2:catch2.bzl", "load_catch2")
load("//third_party/glm:glm.bzl", "load_glm")

def load_third_party_libraries():
    load_catch2()
    load_glm()
