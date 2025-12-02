/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_UTILITY_EXECUTABLE_PATH_HPP
#define PAX_SAPIENTICA_UTILITY_EXECUTABLE_PATH_HPP

#include <string>

#include <PAX_SAPIENTICA/Core/Platform.hpp>

// Platform-specific includes
#if defined(PAXS_PLATFORM_MACOS)
    #include <mach-o/dyld.h>
    #include <libgen.h>
    #include <unistd.h>
    #include <climits>
#elif defined(PAXS_PLATFORM_WINDOWS)
    #include <windows.h>
    #include <direct.h>
#elif defined(PAXS_PLATFORM_LINUX)
    #include <unistd.h>
    #include <libgen.h>
    #include <climits>
#endif

namespace paxs {

    /// @brief 実行ファイルのパス関連のユーティリティクラス
    /// @brief Utility class for executable path operations
    class ExecutablePath {
    public:
        /// @brief 実行ファイルのディレクトリパスを取得する
        /// @brief Get the directory path of the executable file
        /// @return 成功した場合は実行ファイルのディレクトリパス、失敗した場合は空文字列
        /// @return Directory path of the executable file on success, empty string on failure
        static std::string getExecutableDirectory() {
#if defined(PAXS_PLATFORM_MACOS)
            // macOS implementation
            char exe_path[PATH_MAX];
            uint32_t size = sizeof(exe_path);
            if (_NSGetExecutablePath(exe_path, &size) == 0) {
                char* dir = dirname(exe_path);
                if (dir != nullptr) {
                    return std::string(dir);
                }
            }
            return "";

#elif defined(PAXS_PLATFORM_WINDOWS)
            // Windows implementation
            char exe_path[MAX_PATH];
            DWORD length = GetModuleFileNameA(NULL, exe_path, MAX_PATH);
            if (length > 0 && length < MAX_PATH) {
                // パスから最後のバックスラッシュを探す
                // Find the last backslash in the path
                std::string path(exe_path);
                size_t last_slash = path.find_last_of("\\/");
                if (last_slash != std::string::npos) {
                    return path.substr(0, last_slash);
                }
            }
            return "";

#elif defined(PAXS_PLATFORM_LINUX)
            // Linux implementation
            char exe_path[PATH_MAX];
            ssize_t len = readlink("/proc/self/exe", exe_path, sizeof(exe_path) - 1);
            if (len != -1) {
                exe_path[len] = '\0';
                char* dir = dirname(exe_path);
                if (dir != nullptr) {
                    return std::string(dir);
                }
            }
            return "";

#else
            // Unsupported platform
            return "";
#endif
        }

        /// @brief 実行ファイルのディレクトリにカレントディレクトリを変更する
        /// @brief Change the current directory to the executable file's directory
        /// @return 成功した場合はtrue、失敗した場合はfalse
        /// @return true on success, false on failure
        static bool changeToExecutableDirectory() {
            std::string dir = getExecutableDirectory();
            if (dir.empty()) {
                return false;
            }

#if defined(PAXS_PLATFORM_MACOS) || defined(PAXS_PLATFORM_LINUX)
            // macOS and Linux implementation
            return chdir(dir.c_str()) == 0;

#elif defined(PAXS_PLATFORM_WINDOWS)
            // Windows implementation
            return _chdir(dir.c_str()) == 0;

#else
            // Unsupported platform
            return false;
#endif
        }

        /// @brief 実行ファイルのフルパスを取得する
        /// @brief Get the full path of the executable file
        /// @return 成功した場合は実行ファイルのフルパス、失敗した場合は空文字列
        /// @return Full path of the executable file on success, empty string on failure
        static std::string getExecutablePath() {
#if defined(PAXS_PLATFORM_MACOS)
            // macOS implementation
            char exe_path[PATH_MAX];
            uint32_t size = sizeof(exe_path);
            if (_NSGetExecutablePath(exe_path, &size) == 0) {
                return std::string(exe_path);
            }
            return "";

#elif defined(PAXS_PLATFORM_WINDOWS)
            // Windows implementation
            char exe_path[MAX_PATH];
            DWORD length = GetModuleFileNameA(NULL, exe_path, MAX_PATH);
            if (length > 0 && length < MAX_PATH) {
                return std::string(exe_path);
            }
            return "";

#elif defined(PAXS_PLATFORM_LINUX)
            // Linux implementation
            char exe_path[PATH_MAX];
            ssize_t len = readlink("/proc/self/exe", exe_path, sizeof(exe_path) - 1);
            if (len != -1) {
                exe_path[len] = '\0';
                return std::string(exe_path);
            }
            return "";

#else
            // Unsupported platform
            return "";
#endif
        }
    };

} // namespace paxs

#endif // PAX_SAPIENTICA_UTILITY_EXECUTABLE_PATH_HPP

