load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository")

git_repository(
    name = "bazel_rules_qt",
    commit = "a3f6a9ffa9e598e6a5a77d9db427e17c7d1c7ebb",
    remote = "https://github.com/limdor/bazel_rules_qt.git",
    shallow_since = "1584802417 +0100",
)

new_local_repository(
    name = "qt",
    build_file = "@bazel_rules_qt//:qt.BUILD",
    path = "C:\\Qt\\5.9.9\\msvc2017_64\\",
)
