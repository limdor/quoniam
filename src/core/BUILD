load("@com_justbuchanan_rules_qt//:qt.bzl", "qt_cc_library")
load("@rules_cc//cc:defs.bzl", "cc_library")

qt_cc_library(
    name = "gl_canvas",
    srcs = ["GLCanvas.cpp"],
    hdrs = ["GLCanvas.h"],
    local_defines = ["GLM_ENABLE_EXPERIMENTAL"],
    visibility = ["//visibility:public"],
    deps = [
        ":core",
        "@qt//:qt_widgets",
    ],
)

cc_library(
    name = "glsl_shader",
    srcs = ["GLSLShader.cpp"],
    hdrs = ["GLSLShader.h"],
    implementation_deps = ["//src/utils:debug"],
    deps = ["@glew//:glew_static"],
)

cc_library(
    name = "glsl_program",
    srcs = ["GLSLProgram.cpp"],
    hdrs = ["GLSLProgram.h"],
    implementation_deps = ["//src/utils:debug"],
    deps = [
        ":glsl_shader",
        "@glew//:glew_static",
        "@glm",
    ],
)

cc_library(
    name = "texture",
    srcs = ["Texture.cpp"],
    hdrs = ["Texture.h"],
    deps = [
        "@glew//:glew_static",
        "@qt//:qt_gui",
    ],
)

cc_library(
    name = "material",
    srcs = ["Material.cpp"],
    hdrs = ["Material.h"],
    deps = [
        ":texture",
        "@glm",
        "@qt//:qt_gui",
    ],
)

cc_library(
    name = "geometry_topology",
    srcs = ["GeometryTopology.cpp"],
    hdrs = ["GeometryTopology.h"],
    deps = ["@glew//:glew_static"],
)

cc_library(
    name = "light_settings",
    srcs = ["LightSettings.cpp"],
    hdrs = ["LightSettings.h"],
    deps = ["@glm"],
)

cc_library(
    name = "core",
    srcs = [
        "AxisAlignedBoundingBox.cpp",
        "BoundingSphere.cpp",
        "Camera.cpp",
        "GPUGeometry.cpp",
        "GPUScene.cpp",
        "GPUSceneNode.cpp",
        "Geometry.cpp",
        "Gizmo.cpp",
        "Mesh.cpp",
        "OrthographicCamera.cpp",
        "PerspectiveCamera.cpp",
        "Scene.cpp",
        "SceneLoader.cpp",
        "SceneNode.cpp",
        "SerializedSceneGeometry.cpp",
        "TrackballCamera.cpp",
    ],
    hdrs = [
        "AxisAlignedBoundingBox.h",
        "BoundingSphere.h",
        "Camera.h",
        "GPUGeometry.h",
        "GPUScene.h",
        "GPUSceneNode.h",
        "Geometry.h",
        "Gizmo.h",
        "Mesh.h",
        "OrthographicCamera.h",
        "PerspectiveCamera.h",
        "Scene.h",
        "SceneLoader.h",
        "SceneNode.h",
        "SerializedSceneGeometry.h",
        "TrackballCamera.h",
    ],
    includes = ["."],
    local_defines = ["GLM_ENABLE_EXPERIMENTAL"],
    visibility = ["//visibility:public"],
    deps = [
        ":geometry_topology",
        ":glsl_program",
        ":glsl_shader",
        ":light_settings",
        ":material",
        ":texture",
        "//src/utils",
        "//third_party/miniball",
        "@assimp",
        "@glew//:glew_static",
        "@glm",
        "@qt//:qt_core",
        "@qt//:qt_widgets",
    ],
)
