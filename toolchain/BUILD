load("@rules_cc//cc:defs.bzl", "cc_toolchain", "cc_toolchain_suite")
load(":cc_toolchain_config.bzl", "cc_toolchain_config", "semicolon_separated")

package(default_visibility = ["//visibility:public"])

cc_toolchain_suite(
    name = "vs2019_suite",
    toolchains = {
        "x64_windows": ":cc_compiler_x64_windows_vs2019",
    },
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
    supports_param_files = 1,
    toolchain_config = ":msvc_x64",
    toolchain_identifier = "cc_compiler_x64_windows_vs2019",
)

# CI Path
_BUILD_TOOLS_INSTALL_PATH = "C:\\Program Files (x86)\\Microsoft Visual Studio\\2019\\Community\\"
# Local Path
#_BUILD_TOOLS_INSTALL_PATH = "C:\\Program Files (x86)\\Microsoft Visual Studio\\2019\\BuildTools\\"

# CI
_MSVC_VERSION = "14.28.29910"
# Local
#_MSVC_VERSION = "14.28.29333"

_WINDOWS_10_SDK_VERSION = "10.0.19041.0"

_MSVC_INSTALL_PATH = "{}VC\\Tools\\MSVC\\{}\\".format(_BUILD_TOOLS_INSTALL_PATH, _MSVC_VERSION)

_BUILTIN_INCLUDE_DIRECTORIES = [
    "{}include".format(_MSVC_INSTALL_PATH),
    "C:\\Program Files (x86)\\Windows Kits\\10\\include\\{}\\ucrt".format(_WINDOWS_10_SDK_VERSION),
    "C:\\Program Files (x86)\\Windows Kits\\10\\include\\{}\\shared".format(_WINDOWS_10_SDK_VERSION),
    "C:\\Program Files (x86)\\Windows Kits\\10\\include\\{}\\um".format(_WINDOWS_10_SDK_VERSION),
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
        "C:\\Program Files (x86)\\Windows Kits\\10\\lib\\{}\\ucrt\\x64".format(_WINDOWS_10_SDK_VERSION),
        "C:\\Program Files (x86)\\Windows Kits\\10\\lib\\{}\\um\\x64".format(_WINDOWS_10_SDK_VERSION),
    ]),
    msvc_env_path = semicolon_separated([
        "{}bin\\HostX64\\x64".format(_MSVC_INSTALL_PATH),
        "C:\\Program Files (x86)\\Windows Kits\\10\\bin\\{}\\x64".format(_WINDOWS_10_SDK_VERSION),
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
