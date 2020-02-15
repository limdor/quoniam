load("@rules_cc//cc:defs.bzl", "cc_import", "cc_library")

cc_library(
    name = "glew",
    hdrs = glob(["include/*.h"]),
    includes = ["include"],
    visibility = ["//visibility:public"],
    deps = [":glew_import"],
)

cc_import(
    name = "glew_import",
    hdrs = glob(["include/*.h"]),
    interface_library = "lib/glew32.lib",
    shared_library = "bin/glew32.dll",
    visibility = ["//visibility:public"],
)
