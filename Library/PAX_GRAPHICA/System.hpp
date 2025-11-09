/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_GRAPHICA_SYSTEM_HPP
#define PAX_GRAPHICA_SYSTEM_HPP

#include <string>

#if defined(PAXS_USING_SIV3D)
#include <Siv3D.hpp>
#elif defined(PAXS_USING_SFML)
#include <cstdlib>
#elif defined(PAXS_USING_DXLIB)
#include <cstdlib>
#ifdef __ANDROID__
#include <jni.h>
#include <android/native_activity.h>
#endif
#endif

namespace paxg {

    /// @brief システム関連の機能を提供するクラス
    class System {
    public:
        /// @brief URLをブラウザで開く
        /// @param url 開くURL
        /// @return 成功した場合true
        static bool launchBrowser(const std::string& url) {
#if defined(PAXS_USING_SIV3D)
            return s3d::System::LaunchBrowser(s3d::Unicode::FromUTF8(url));
#elif defined(PAXS_USING_SFML)
            // macOS
#ifdef __APPLE__
            std::string command = "open \"" + url + "\"";
            return std::system(command.c_str()) == 0;
#elif defined(_WIN32)
            // Windows
            std::string command = "start \"\" \"" + url + "\"";
            return std::system(command.c_str()) == 0;
#elif defined(__linux__)
            // Linux
            std::string command = "xdg-open \"" + url + "\"";
            return std::system(command.c_str()) == 0;
#else
            (void)url;
            return false;
#endif
#elif defined(PAXS_USING_DXLIB)
#ifdef __ANDROID__
            // Android - JNI経由でブラウザを開く
            // Note: 実装には ANativeActivity へのアクセスが必要
            // 現時点ではスタブとして false を返す
            (void)url;
            return false;
#else
            // Windows/その他のDxLib環境
#ifdef _WIN32
            std::string command = "start \"\" \"" + url + "\"";
            return std::system(command.c_str()) == 0;
#else
            (void)url;
            return false;
#endif
#endif
#else
            (void)url;
            return false;
#endif
        }
    };

} // namespace paxg

#endif // !PAX_GRAPHICA_SYSTEM_HPP
