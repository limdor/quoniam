load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository")
load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")
load("//third_party:third_party.bzl", "load_third_party_libraries")

load_third_party_libraries()

git_repository(
    name = "bazel_rules_qt",
    commit = "716d61bfdcee9fc2de0f641c087e28584e8fddde",
    remote = "https://github.com/limdor/bazel_rules_qt.git",
    shallow_since = "1608662603 +0100",
)

new_local_repository(
    name = "qt",
    build_file = "@bazel_rules_qt//:qt.BUILD",
    path = "C:\\Qt\\5.9.9\\msvc2017_64\\",
)

http_archive(
    name = "rules_pkg",
    sha256 = "6b5969a7acd7b60c02f816773b06fcf32fbe8ba0c7919ccdc2df4f8fb923804a",
    url = "https://github.com/bazelbuild/rules_pkg/releases/download/0.3.0/rules_pkg-0.3.0.tar.gz",
)

load("@rules_pkg//:deps.bzl", "rules_pkg_dependencies")

rules_pkg_dependencies()
