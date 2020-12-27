"""
Transitive dependencies for rules_pkg
"""

load("@rules_pkg//:deps.bzl", "rules_pkg_dependencies")

def load_rules_pkg_transitive_dependencies():
    rules_pkg_dependencies()
