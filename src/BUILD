load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_library")
load("@bazel_rules_qt//:qt.bzl", "qt_cc_library", "qt_ui_library")

qt_ui_library(
    name = "dutagaci_view_ui",
    ui = "DutagaciView.ui",
    deps = ["@qt//:qt_widgets"],
)

qt_cc_library(
    name = "dutagaci_dialog",
    srcs = ["DutagaciDialog.cpp"],
    hdrs = ["DutagaciDialog.h"],
    deps = [
        ":dutagaci_view_ui",
        ":quoniam_lib",
        "@qt//:qt_widgets",
    ],
)

qt_ui_library(
    name = "main_module_ui",
    includes = ["."],
    ui = "MainModule.ui",
    deps = ["@qt//:qt_widgets"],
)

qt_cc_library(
    name = "main_module_controller",
    srcs = ["MainModuleController.cpp"],
    hdrs = ["MainModuleController.h"],
    deps = [
        ":dutagaci_dialog",
        ":main_module_ui",
        ":quoniam_lib",
        ":viewpoint_measure_slider",
        "@qt//:qt_widgets",
    ],
)

qt_ui_library(
    name = "main_window_ui",
    ui = "MainWindow.ui",
    deps = ["@qt//:qt_widgets"],
)

qt_cc_library(
    name = "main_window",
    srcs = ["MainWindow.cpp"],
    hdrs = ["MainWindow.h"],
    deps = [
        ":dutagaci_dialog",
        ":main_module_controller",
        ":main_window_ui",
        ":quoniam_lib",
        "@qt//:qt_widgets",
    ],
)

qt_cc_library(
    name = "module_controller",
    srcs = ["ModuleController.cpp"],
    hdrs = ["ModuleController.h"],
    deps = ["@qt//:qt_widgets"],
)

qt_cc_library(
    name = "module_tab_widget",
    srcs = ["ModuleTabWidget.cpp"],
    hdrs = ["ModuleTabWidget.h"],
    deps = ["@qt//:qt_widgets"],
)

qt_cc_library(
    name = "viewpoint_measure_slider",
    srcs = ["ViewpointMeasureSlider.cpp"],
    hdrs = ["ViewpointMeasureSlider.h"],
    deps = ["@qt//:qt_widgets"],
)

cc_binary(
    name = "quoniam",
    srcs = ["Main.cpp"],
    deps = [
        ":main_module_controller",
        ":main_window",
        ":module_controller",
        ":module_tab_widget",
        ":quoniam_lib",
        "//src/core:gl_canvas",
    ],
)

cc_library(
    name = "quoniam_lib",
    srcs = [
        "Measure.cpp",
        "Measure.h",
        "ProjectedAreasMatrix.cpp",
        "ProjectedAreasMatrix.h",
        "SceneInformationBuilder.cpp",
        "SceneInformationBuilder.h",
        "SphereOfViewpoints.cpp",
        "SphereOfViewpoints.h",
        "SpherePointCloud.cpp",
        "SpherePointCloud.h",
        "viewpoint-measures/CurvatureEntropy.cpp",
        "viewpoint-measures/CurvatureEntropy.h",
        "viewpoint-measures/DepthBasedVisualStability.cpp",
        "viewpoint-measures/DepthBasedVisualStability.h",
        "viewpoint-measures/DepthDistribution.cpp",
        "viewpoint-measures/DepthDistribution.h",
        "viewpoint-measures/FeixasSaliency.cpp",
        "viewpoint-measures/FeixasSaliency.h",
        "viewpoint-measures/HeuristicMeasure.cpp",
        "viewpoint-measures/HeuristicMeasure.h",
        "viewpoint-measures/I2.cpp",
        "viewpoint-measures/I2.h",
        "viewpoint-measures/I3.cpp",
        "viewpoint-measures/I3.h",
        "viewpoint-measures/KullbackLeibler.cpp",
        "viewpoint-measures/KullbackLeibler.h",
        "viewpoint-measures/MaximumDepth.cpp",
        "viewpoint-measures/MaximumDepth.h",
        "viewpoint-measures/NumberOfVisibleTriangles.cpp",
        "viewpoint-measures/NumberOfVisibleTriangles.h",
        "viewpoint-measures/ProjectedArea.cpp",
        "viewpoint-measures/ProjectedArea.h",
        "viewpoint-measures/SaliencyEVMI.cpp",
        "viewpoint-measures/SaliencyEVMI.h",
        "viewpoint-measures/SilhouetteCurvature.cpp",
        "viewpoint-measures/SilhouetteCurvature.h",
        "viewpoint-measures/SilhouetteCurvatureExtrema.cpp",
        "viewpoint-measures/SilhouetteCurvatureExtrema.h",
        "viewpoint-measures/SilhouetteEntropy.cpp",
        "viewpoint-measures/SilhouetteEntropy.h",
        "viewpoint-measures/SilhouetteLength.cpp",
        "viewpoint-measures/SilhouetteLength.h",
        "viewpoint-measures/StoevStrasser.cpp",
        "viewpoint-measures/StoevStrasser.h",
        "viewpoint-measures/Unstability.cpp",
        "viewpoint-measures/Unstability.h",
        "viewpoint-measures/VMI.cpp",
        "viewpoint-measures/VMI.h",
        "viewpoint-measures/ViewpointEntropy.cpp",
        "viewpoint-measures/ViewpointEntropy.h",
        "viewpoint-measures/VisibilityRatio.cpp",
        "viewpoint-measures/VisibilityRatio.h",
    ],
    defines = ['APP_VERSION=\\"1.0.1\\"'],
    includes = [
        ".",
        "viewpoint-measures/",
    ],
    linkopts = [
        "-DEFAULTLIB:opengl32.lib",
        "-DEFAULTLIB:glu32.lib",
    ],
    deps = [
        ":main_window_ui",
        "//dependencies/glew",
        "//dependencies/glm",
        "//dependencies/opencv",
        "//src/core",
        "//src/utils",
        "@qt//:qt_core",
        "@qt//:qt_gui",
        "@qt//:qt_opengl",
        "@qt//:qt_widgets",
    ],
)
