"""
File to load the dependencies of the dependencies
This could be done when loading the specific dependency, the problem with that approach would
be that we would not have control over the dependency version.
In this way if a third party library depends on some package that we also depend on, first we
load the package with the version that we want, and then the third party library has already the
package.
"""

load("//third_party/bazel_skylib:transitive.bzl", "load_bazel_skylib_transitive_dependencies")
load("//third_party/catch2:transitive.bzl", "load_catch2_transitive_dependencies")
load("//third_party/glew:transitive.bzl", "load_glew_transitive_dependencies")
load("//third_party/glm:transitive.bzl", "load_glm_transitive_dependencies")
load("//third_party/rules_pkg:transitive.bzl", "load_rules_pkg_transitive_dependencies")
load("//third_party/rules_python:transitive.bzl", "load_rules_python_transitive_dependencies")
load("//third_party/rules_qt:transitive.bzl", "load_rules_qt_transitive_dependencies")

def load_transitive_dependencies():
    """Load the transitive dependencies of only our direct dependencies"""
    load_bazel_skylib_transitive_dependencies()
    load_catch2_transitive_dependencies()
    load_glew_transitive_dependencies()
    load_glm_transitive_dependencies()
    load_rules_pkg_transitive_dependencies()
    load_rules_python_transitive_dependencies()
    load_rules_qt_transitive_dependencies()