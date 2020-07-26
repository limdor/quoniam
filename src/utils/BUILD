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
    copts = ["/std:c++latest"],
    includes = ["."],
    linkopts = [
        "-DEFAULTLIB:glu32.lib",
        "-DEFAULTLIB:opengl32.lib",
    ],
    visibility = ["//visibility:public"],
    deps = [
        "//dependencies/glew",
        "@glm",
        "@qt//:qt_core",
        "@qt//:qt_gui",
        "@qt//:qt_widgets",
    ],
)
