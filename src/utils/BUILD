load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "debug",
    srcs = ["Debug.cpp"],
    hdrs = ["Debug.h"],
    copts = ["/std:c++latest"],
    includes = ["."],
    linkopts = [
        "-DEFAULTLIB:glu32.lib",
        "-DEFAULTLIB:opengl32.lib",
    ],
    visibility = ["//visibility:public"],
    deps = [
        "@glew",
        "@qt//:qt_widgets",
    ],
)

cc_library(
    name = "tools",
    srcs = ["Tools.cpp"],
    hdrs = ["Tools.h"],
    copts = ["/std:c++latest"],
    includes = ["."],
    visibility = ["//visibility:public"],
    deps = [
        ":debug",
        "@glm",
        "@qt//:qt_core",
    ],
)

cc_library(
    name = "utils",
    visibility = ["//visibility:public"],
    deps = [
        ":tools",
        ":debug",
    ],
)
