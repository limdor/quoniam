load("@rules_cc//cc:defs.bzl", "cc_test")

cc_test(
    name = "tools",
    size = "small",
    srcs = ["tst_Tools.cpp"],
    copts = ["/std:c++latest"],
    deps = [
        "//src/utils",
        "@catch2",
    ],
)
