"""
Load all third party dependencies needed for this project
"""

load("//third_party/rules_qt:direct.bzl", "load_rules_qt")

def load_third_party_libraries():
    """Load all third party dependencies"""
    load_rules_qt()
