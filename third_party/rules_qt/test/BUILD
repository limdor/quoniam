load("@rules_cc//cc:defs.bzl", "cc_test")

cc_test(
    name = "smoke_test",
    size = "small",
    srcs = ["main.cpp"],
    deps = [
        "@catch2",
        "@qt//:qt_core",
        "@catch2//:catch2_main",
    ],
)
