cc_library(
    name = "assimp",
    hdrs = glob(["include/assimp/*.h", "include/assimp/*.hpp"]),
    includes = ["include"],
    deps = [":assimp_import"],
    visibility = ["//visibility:public"],
)

cc_import(
    name = "assimp_import",
    hdrs = glob(["include/assimp/*.h", "include/assimp/*.hpp"]),
    interface_library = select({
        "//config:release": "lib/release/assimp.lib",
        "//config:debug": "lib/debug/assimp.lib",
        "//conditions:default": "lib/release/assimp.lib",
    }),
    shared_library = select({
        "//config:release": "bin/release/assimp.dll",
        "//config:debug": "bin/debug/assimp.dll",
        "//conditions:default": "bin/release/assimp.dll",
    }),
    visibility = ["//visibility:public"],
)
