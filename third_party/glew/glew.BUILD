load("@rules_cc//cc:defs.bzl", "cc_import", "cc_library")

cc_library(
    name = "glew",
    hdrs = glob(["include/GL/*.h"]),
    includes = ["include/GL/"],
    visibility = ["//visibility:public"],
    deps = [":glew_import"],
)

cc_import(
    name = "glew_import",
    hdrs = glob(["include/GL/*.h"]),
    interface_library = "lib/Release/x64/glew32.lib",
    shared_library = "bin/Release/x64/glew32.dll",
)
