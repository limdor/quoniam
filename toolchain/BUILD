load("@rules_cc//cc:defs.bzl", "cc_toolchain", "cc_toolchain_suite")
load(":cc_toolchain_config.bzl", "cc_toolchain_config")

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

cc_toolchain_config(
    name = "msvc_x64",
    abi_libc_version = "local",
    abi_version = "local",
    archiver_flags = ["/MACHINE:X64"],
    compiler = "msvc-cl",
    cpu = "x64_windows",
    cxx_builtin_include_directories = [
        "C:\\Program Files (x86)\\Microsoft Visual Studio\\2019\\BuildTools\\VC\\Tools\\MSVC\\14.24.28314\\include",
        "C:\\Program Files (x86)\\Windows Kits\\10\\include\\10.0.10240.0\\ucrt",
        "C:\\Program Files (x86)\\Windows Kits\\8.1\\include\\\\shared",
        "C:\\Program Files (x86)\\Windows Kits\\8.1\\include\\\\um",
        "C:\\Program Files (x86)\\Windows Kits\\8.1\\include\\\\winrt",
        "C:\\Program Files (x86)\\Windows Kits\\8.1\\include\\\\cppwinrt",
    ],
    dbg_mode_debug_flag = "/DEBUG:FULL",
    default_link_flags = ["/MACHINE:X64"],
    fastbuild_mode_debug_flag = "/DEBUG:FASTLINK",
    host_system_name = "local",
    msvc_cl_path = "C:/Program Files (x86)/Microsoft Visual Studio/2019/BuildTools/VC/Tools/MSVC/14.24.28314/bin/HostX64/x64/cl.exe",
    msvc_env_include = "C:\\Program Files (x86)\\Microsoft Visual Studio\\2019\\BuildTools\\VC\\Tools\\MSVC\\14.24.28314\\include;C:\\Program Files (x86)\\Windows Kits\\10\\include\\10.0.10240.0\\ucrt;C:\\Program Files (x86)\\Windows Kits\\8.1\\include\\\\shared;C:\\Program Files (x86)\\Windows Kits\\8.1\\include\\\\um;C:\\Program Files (x86)\\Windows Kits\\8.1\\include\\\\winrt;C:\\Program Files (x86)\\Windows Kits\\8.1\\include\\\\cppwinrt",
    msvc_env_lib = "C:\\Program Files (x86)\\Microsoft Visual Studio\\2019\\BuildTools\\VC\\Tools\\MSVC\\14.24.28314\\lib\\x64;C:\\Program Files (x86)\\Windows Kits\\10\\lib\\10.0.10240.0\\ucrt\\x64;C:\\Program Files (x86)\\Windows Kits\\8.1\\lib\\winv6.3\\um\\x64",
    msvc_env_path = "C:\\Program Files (x86)\\Microsoft Visual Studio\\2019\\BuildTools\\VC\\Tools\\MSVC\\14.24.28314\\bin\\HostX64\\x64;C:\\Program Files (x86)\\Microsoft Visual Studio\\2019\\BuildTools\\MSBuild\\Current\\bin\\Roslyn;C:\\Program Files (x86)\\Windows Kits\\8.1\\bin\\x64;C:\\Program Files (x86)\\Microsoft Visual Studio\\2019\\BuildTools\\\\MSBuild\\Current\\Bin;C:\\Windows\\Microsoft.NET\\Framework64\\v4.0.30319;C:\\Program Files (x86)\\Microsoft Visual Studio\\2019\\BuildTools\\Common7\\IDE\\;C:\\Program Files (x86)\\Microsoft Visual Studio\\2019\\BuildTools\\Common7\\Tools\\;;C:\\WINDOWS\\system32",
    msvc_env_tmp = "C:\\Users\\xavib\\AppData\\Local\\Temp",
    msvc_lib_path = "C:/Program Files (x86)/Microsoft Visual Studio/2019/BuildTools/VC/Tools/MSVC/14.24.28314/bin/HostX64/x64/lib.exe",
    msvc_link_path = "C:/Program Files (x86)/Microsoft Visual Studio/2019/BuildTools/VC/Tools/MSVC/14.24.28314/bin/HostX64/x64/link.exe",
    msvc_ml_path = "C:/Program Files (x86)/Microsoft Visual Studio/2019/BuildTools/VC/Tools/MSVC/14.24.28314/bin/HostX64/x64/ml64.exe",
    target_libc = "msvcrt",
    target_system_name = "local",
    tool_paths = {
        "ar": "C:/Program Files (x86)/Microsoft Visual Studio/2019/BuildTools/VC/Tools/MSVC/14.24.28314/bin/HostX64/x64/lib.exe",
        "ml": "C:/Program Files (x86)/Microsoft Visual Studio/2019/BuildTools/VC/Tools/MSVC/14.24.28314/bin/HostX64/x64/ml64.exe",
        "cpp": "C:/Program Files (x86)/Microsoft Visual Studio/2019/BuildTools/VC/Tools/MSVC/14.24.28314/bin/HostX64/x64/cl.exe",
        "gcc": "C:/Program Files (x86)/Microsoft Visual Studio/2019/BuildTools/VC/Tools/MSVC/14.24.28314/bin/HostX64/x64/cl.exe",
        "gcov": "wrapper/bin/msvc_nop.bat",
        "ld": "C:/Program Files (x86)/Microsoft Visual Studio/2019/BuildTools/VC/Tools/MSVC/14.24.28314/bin/HostX64/x64/link.exe",
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
