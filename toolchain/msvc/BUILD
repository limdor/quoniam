load("@rules_cc//cc:defs.bzl", "cc_toolchain")
load("//toolchain:utils.bzl", "semicolon_separated")
load(":cc_toolchain_config.bzl", "cc_toolchain_config")

package(default_visibility = ["//visibility:public"])

toolchain(
    name = "msvc_2019_windows_toolchain",
    exec_compatible_with = [
        "@platforms//os:windows",
        "@platforms//cpu:x86_64",
    ],
    target_compatible_with = [
        "@platforms//os:windows",
        "@platforms//cpu:x86_64",
    ],
    toolchain = ":cc_compiler_x64_windows_vs2019",
    toolchain_type = "@bazel_tools//tools/cpp:toolchain_type",
)

cc_toolchain(
    name = "cc_compiler_x64_windows_vs2019",
    all_files = ":empty",
    ar_files = ":empty",
    as_files = ":empty",
    compiler_files = ":empty",
    dwp_files = ":empty",
    linker_files = ":empty",
    objcopy_files = ":empty",
    strip_files = ":empty",
    supports_param_files = True,
    toolchain_config = ":msvc_x64",
    toolchain_identifier = "cc_compiler_x64_windows_vs2019",
)

# CI Path
_BUILD_TOOLS_INSTALL_PATH = "C:\\Program Files (x86)\\Microsoft Visual Studio\\2019\\Community\\"
# Local Path
#_BUILD_TOOLS_INSTALL_PATH = "C:\\Program Files (x86)\\Microsoft Visual Studio\\2019\\BuildTools\\"

_MSVC_VERSION = "14.29.30133"

_WINDOWS_SDK_MAJOR_VERSION = "10"
_WINDOWS_SDK_MINOR_VERSION = "0"
_WINDOWS_SDK_BUILD_NUMBER = "19041.0"

_WINDOWS_KIT_INSTALL_PATH = "C:\\Program Files (x86)\\Windows Kits\\{}\\".format(_WINDOWS_SDK_MAJOR_VERSION)

_WINDOWS_SDK_FULL_VERSION = "{}.{}.{}".format(_WINDOWS_SDK_MAJOR_VERSION, _WINDOWS_SDK_MINOR_VERSION, _WINDOWS_SDK_BUILD_NUMBER)

_MSVC_INSTALL_PATH = "{}VC\\Tools\\MSVC\\{}\\".format(_BUILD_TOOLS_INSTALL_PATH, _MSVC_VERSION)

_BUILTIN_INCLUDE_DIRECTORIES = [
    "{}include".format(_MSVC_INSTALL_PATH),
    "{}include\\{}\\ucrt".format(_WINDOWS_KIT_INSTALL_PATH, _WINDOWS_SDK_FULL_VERSION),
    "{}include\\{}\\shared".format(_WINDOWS_KIT_INSTALL_PATH, _WINDOWS_SDK_FULL_VERSION),
    "{}include\\{}\\um".format(_WINDOWS_KIT_INSTALL_PATH, _WINDOWS_SDK_FULL_VERSION),
]

cc_toolchain_config(
    name = "msvc_x64",
    abi_libc_version = "local",
    abi_version = "local",
    archiver_flags = ["/MACHINE:X64"],
    compiler = "msvc-cl",
    cpu = "x64_windows",
    cxx_builtin_include_directories = _BUILTIN_INCLUDE_DIRECTORIES,
    dbg_mode_debug_flag = "/DEBUG:FULL",
    default_link_flags = ["/MACHINE:X64"],
    fastbuild_mode_debug_flag = "/DEBUG:FASTLINK",
    host_system_name = "local",
    msvc_cl_path = "{}bin/HostX64/x64/cl.exe".format(_MSVC_INSTALL_PATH),
    msvc_env_include = semicolon_separated(_BUILTIN_INCLUDE_DIRECTORIES),
    msvc_env_lib = semicolon_separated([
        "{}lib\\x64".format(_MSVC_INSTALL_PATH),
        "{}lib\\{}\\ucrt\\x64".format(_WINDOWS_KIT_INSTALL_PATH, _WINDOWS_SDK_FULL_VERSION),
        "{}lib\\{}\\um\\x64".format(_WINDOWS_KIT_INSTALL_PATH, _WINDOWS_SDK_FULL_VERSION),
    ]),
    msvc_env_path = semicolon_separated([
        "{}bin\\HostX64\\x64".format(_MSVC_INSTALL_PATH),
        "{}bin\\{}\\x64".format(_WINDOWS_KIT_INSTALL_PATH, _WINDOWS_SDK_FULL_VERSION),
        "{}MSBuild\\Current\\Bin".format(_BUILD_TOOLS_INSTALL_PATH),
        "{}Common7\\IDE\\".format(_BUILD_TOOLS_INSTALL_PATH),
        "{}Common7\\Tools\\".format(_BUILD_TOOLS_INSTALL_PATH),
        "C:\\WINDOWS\\system32",
    ]),
    msvc_env_tmp = "C:\\Users\\xavib\\AppData\\Local\\Temp",
    msvc_lib_path = "{}bin/HostX64/x64/lib.exe".format(_MSVC_INSTALL_PATH),
    msvc_link_path = "{}bin/HostX64/x64/link.exe".format(_MSVC_INSTALL_PATH),
    msvc_ml_path = "{}bin/HostX64/x64/ml64.exe".format(_MSVC_INSTALL_PATH),
    target_libc = "msvcrt",
    target_system_name = "local",
    tool_paths = {
        "ar": "{}bin/HostX64/x64/lib.exe".format(_MSVC_INSTALL_PATH),
        "ml": "{}bin/HostX64/x64/ml64.exe".format(_MSVC_INSTALL_PATH),
        "cpp": "{}bin/HostX64/x64/cl.exe".format(_MSVC_INSTALL_PATH),
        "gcc": "{}bin/HostX64/x64/cl.exe".format(_MSVC_INSTALL_PATH),
        "gcov": "wrapper/bin/msvc_nop.bat",
        "ld": "{}bin/HostX64/x64/link.exe".format(_MSVC_INSTALL_PATH),
        "nm": "wrapper/bin/msvc_nop.bat",
        "objcopy": "wrapper/bin/msvc_nop.bat",
        "objdump": "wrapper/bin/msvc_nop.bat",
        "strip": "wrapper/bin/msvc_nop.bat",
    },
    toolchain_identifier = "msvc_x64",
)

filegroup(
    name = "empty",
    srcs = [],
)
