cc_library(
    name = "glew",
    hdrs = glob(["include/*.h"]),
    includes = ["include"],
    deps = [":glew_import"],
    visibility = ["//visibility:public"],
)

cc_import(
    name = "glew_import",
    hdrs = glob(["include/*.h"]),
    interface_library = "lib/glew32.lib",
    shared_library = "bin/glew32.dll",
    visibility = ["//visibility:public"],
)
