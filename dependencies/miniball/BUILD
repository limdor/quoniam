cc_library(
    name = "miniball",
    hdrs = ["Miniball.h"],
    includes = ["."],
    visibility = ["//visibility:public"],
)

cc_test(
    name = "smoke_test",
    srcs = ["test/main.cpp"],
    deps = [
        "//dependencies/catch2",
        ":miniball",
    ],
)
