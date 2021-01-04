load("@rules_cc//cc:defs.bzl", "cc_test")

cc_test(
    name = "smoke_test",
    size = "small",
    srcs = ["main.cpp"],
    deps = [
        "//third_party/assimp",
        "@catch2",
    ],
)
