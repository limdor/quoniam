load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "utils",
    srcs = [
        "Debug.cpp",
        "Tools.cpp",
    ],
    hdrs = [
        "Debug.h",
        "Tools.h",
    ],
    includes = ["."],
    visibility = ["//visibility:public"],
    deps = [
        "//dependencies/glew",
        "//dependencies/glm",
        "@qt//:qt_core",
        "@qt//:qt_widgets",
    ],
)
