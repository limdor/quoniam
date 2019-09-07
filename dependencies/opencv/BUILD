cc_library(
    name = "opencv",
    hdrs = glob(["include/**/*.h", "include/**/*.hpp"]),
    includes = ["include"],
    deps = [
        ":opencv_core",
        ":opencv_imgproc",
        ":opencv_imgcodecs",
        ":opencv_highgui",
    ],
    visibility = ["//visibility:public"],
)

[
    cc_import(
        name = "opencv_%s" % library,
        hdrs = glob(["include/**/*.h", "include/**/*.hpp"]),
        interface_library = select({
            "//config:release": "lib/release/opencv_%s331.lib" % library,
            "//config:debug": "lib/debug/opencv_%s331d.lib" % library,
            "//conditions:default": "lib/release/opencv_%s331.lib" % library,
        }),
        shared_library = select({
            "//config:release": "bin/release/opencv_%s331.dll" % library,
            "//config:debug": "bin/debug/opencv_%s331d.dll" % library,
            "//conditions:default": "bin/release/opencv_%s331.dll" % library,
        }),
        visibility = ["//visibility:public"],
    ) for library in ('core', 'imgproc', 'imgcodecs', 'highgui')
]
