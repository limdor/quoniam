load("@rules_pkg//:pkg.bzl", "pkg_tar")

filegroup(
    name = "shaders",
    srcs = glob([
        "*.vert",
        "*.frag",
    ]),
)

pkg_tar(
    name = "shaders-pkg",
    srcs = [":shaders"],
    mode = "0644",
    package_dir = "/shaders",
    visibility = ["//visibility:public"],
)
