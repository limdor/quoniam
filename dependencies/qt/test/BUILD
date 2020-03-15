load("@rules_cc//cc:defs.bzl", "cc_test")

cc_test(
    name = "smoke_test",
    size = "small",
    srcs = ["main.cpp"],
    deps = [
        "@qt//:qt_core",
        "//dependencies/catch2",
    ],
)
