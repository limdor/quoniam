cc_library(
    name = "glm",
    hdrs = glob(["glm/**/*.hpp"]),
    includes = ["."],
    visibility = ["//visibility:public"],
)

cc_test(
    name = "smoke_test",
    srcs = ["test/main.cpp"],
    deps = [
        "//dependencies/catch2",
        ":glm",
    ],
)
