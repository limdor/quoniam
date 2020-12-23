load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")
load("//third_party:third_party.bzl", "load_third_party_libraries")

load_third_party_libraries()

http_archive(
    name = "com_limdor_rules_qt",
    sha256 = "94e7b9c77620433185688a418c87efc18524fcd3a22b83d520942fd62e6983d6",
    strip_prefix = "bazel_rules_qt-0.1.0",
    url = "https://github.com/limdor/bazel_rules_qt/archive/v0.1.0.tar.gz",
)

new_local_repository(
    name = "qt",
    build_file = "@com_limdor_rules_qt//:qt.BUILD",
    path = "C:\\Qt\\5.9.9\\msvc2017_64\\",
)

http_archive(
    name = "rules_pkg",
    sha256 = "6b5969a7acd7b60c02f816773b06fcf32fbe8ba0c7919ccdc2df4f8fb923804a",
    url = "https://github.com/bazelbuild/rules_pkg/releases/download/0.3.0/rules_pkg-0.3.0.tar.gz",
)

load("@rules_pkg//:deps.bzl", "rules_pkg_dependencies")

rules_pkg_dependencies()
