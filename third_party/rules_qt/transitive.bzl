"""
Transitive dependencies for rules_qt
"""

def load_rules_qt_transitive_dependencies():
    native.new_local_repository(
        name = "qt",
        build_file = "@com_justbuchanan_rules_qt//:qt.BUILD",
        path = "C:\\Qt\\5.9.9\\msvc2017_64\\",
    )
