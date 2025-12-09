/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_GRAPHICA_NETWORK_HPP
#define PAX_GRAPHICA_NETWORK_HPP

#include <string>

#include <PAX_SAPIENTICA/Core/Platform.hpp>
#include <PAX_SAPIENTICA/IO/File/FileSystem.hpp>
#include <PAX_SAPIENTICA/System/AppConfig.hpp>

#if defined(PAXS_USING_SIV3D)
#include <Siv3D.hpp>

#elif defined(PAXS_USING_SFML)
#include <cstdlib>
#include <fstream>
#include <filesystem>

#if defined(PAXS_PLATFORM_MACOS)
#include <curl/curl.h>

#elif defined(PAXS_PLATFORM_WINDOWS)
#include <windows.h>
#include <wininet.h>
#pragma comment(lib, "wininet.lib")

#elif defined(PAXS_PLATFORM_LINUX)
#include <curl/curl.h>

#endif // PAXS_PLATFORM_MACOS, PAXS_PLATFORM_WINDOWS, PAXS_PLATFORM_LINUX

#elif defined(PAXS_USING_DXLIB)
#include <cstdlib>

#endif // PAXS_USING_SIV3D, PAXS_USING_SFML, PAXS_USING_DXLIB

namespace paxg {

    /// @brief ネットワーク関連の機能を提供するクラス
    class Network {
    public:
        /// @brief URLをブラウザで開く
        /// @param url 開くURL
        /// @return 成功した場合true
        static bool launchBrowser(const std::string& url) {
#if defined(PAXS_USING_SIV3D)
            return s3d::System::LaunchBrowser(s3d::Unicode::FromUTF8(url));
#elif defined(PAXS_USING_SFML)
            // macOS
#if defined(PAXS_PLATFORM_MACOS)
            std::string command = "open \"" + url + "\"";
            return std::system(command.c_str()) == 0;
#elif defined(PAXS_PLATFORM_WINDOWS)
            // Windows
            std::string command = "start \"\" \"" + url + "\"";
            return std::system(command.c_str()) == 0;
#elif defined(PAXS_PLATFORM_LINUX)
            // Linux
            std::string command = "xdg-open \"" + url + "\"";
            return std::system(command.c_str()) == 0;
#else
            (void)url;
            return false;
#endif
#elif defined(PAXS_USING_DXLIB)
#if defined(PAXS_PLATFORM_ANDROID)
            // Android - JNI経由でブラウザを開く
            // Note: 実装には ANativeActivity へのアクセスが必要
            // 現時点ではスタブとして false を返す
            (void)url;
            return false;
#else
            // Windows/その他のDxLib環境
#if defined(PAXS_PLATFORM_WINDOWS)
            std::string command = "start \"\" \"" + url + "\"";
            return std::system(command.c_str()) == 0;
#else
            (void)url;
            return false;
#endif // PAXS_PLATFORM_WINDOWS
#endif // PAXS_PLATFORM_ANDROID
#else
            (void)url;
            return false;
#endif
        }

        /// @brief URLからファイルをダウンロードして保存する
        /// @param url ダウンロード元のURL
        /// @param save_path 保存先のファイルパス（アセットルートからの相対パス）
        /// @return 成功した場合true
        static bool downloadFile(const std::string& url, const std::string& save_path) {
#if defined(PAXS_USING_SIV3D)
            const s3d::URL siv_url = s3d::Unicode::FromUTF8(url);
            return s3d::SimpleHTTP::Save(siv_url, s3d::Unicode::FromUTF8(save_path)).isOK();
#elif defined(PAXS_USING_SFML)

#if defined(PAXS_PLATFORM_MACOS)
            // macOS: libcurl を使用
            return downloadWithCurl(url, save_path);
#elif defined(PAXS_PLATFORM_WINDOWS)
            // Windows: WinINet を使用
            return downloadWithWinINet(url, save_path);
#elif defined(PAXS_PLATFORM_LINUX)
            // Linux: libcurl を使用
            return downloadWithCurl(url, save_path);
#else
            (void)url;
            (void)save_path;
            return false;
#endif // PAXS_PLATFORM_MACOS, PAXS_PLATFORM_WINDOWS, PAXS_PLATFORM_LINUX
#else
            (void)url;
            (void)save_path;
            return false;
#endif // PAXS_USING_SIV3D, PAXS_USING_SFML
        }

    private:
#if defined(PAXS_USING_SFML)
#if defined(PAXS_PLATFORM_MACOS) || defined(PAXS_PLATFORM_LINUX)
        // libcurl を使用したダウンロード
        static size_t writeCallback(void* contents, size_t size, size_t nmemb, void* userp) {
            std::ofstream* out = static_cast<std::ofstream*>(userp);
            size_t total_size = size * nmemb;
            out->write(static_cast<char*>(contents), total_size);
            return total_size;
        }

        static bool downloadWithCurl(const std::string& url, const std::string& save_path) {
            CURL* curl = curl_easy_init();
            if (!curl) return false;

            // 相対パスを絶対パスに変換
            const std::string root_path = paxs::AppConfig::getInstance().getRootPath();
            const std::string full_path = root_path + save_path;

            // 保存先のディレクトリを作成（相対パスで指定）
            std::filesystem::path path(save_path);
            if (path.has_parent_path()) {
                if (!paxs::FileSystem::createDirectories(path.parent_path().string())) {
                    curl_easy_cleanup(curl);
                    return false;
                }
            }

            std::ofstream out_file(full_path, std::ios::binary);
            if (!out_file.is_open()) {
                curl_easy_cleanup(curl);
                return false;
            }

            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &out_file);
            curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
            curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L); // SSL証明書の検証をスキップ（開発用）
            curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);

            CURLcode res = curl_easy_perform(curl);

            out_file.close();
            curl_easy_cleanup(curl);

            if (res != CURLE_OK) {
                std::filesystem::remove(full_path); // 失敗時はファイルを削除
                return false;
            }

            return true;
        }
#endif // PAXS_PLATFORM_MACOS, PAXS_PLATFORM_LINUX

#if defined(PAXS_PLATFORM_WINDOWS)
        // WinINet を使用したダウンロード（Windows）
        static bool downloadWithWinINet(const std::string& url, const std::string& save_path) {
            // 相対パスを絶対パスに変換
            const std::string root_path = paxs::AppConfig::getInstance().getRootPath();
            const std::string full_path = root_path + save_path;

            // 保存先のディレクトリを作成（相対パスで指定）
            std::filesystem::path path(save_path);
            if (path.has_parent_path()) {
                if (!paxs::FileSystem::createDirectories(path.parent_path().string())) {
                    return false;
                }
            }

            HINTERNET hInternet = InternetOpenA("PAX_SAPIENTICA", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
            if (!hInternet) return false;

            HINTERNET hUrl = InternetOpenUrlA(hInternet, url.c_str(), NULL, 0, INTERNET_FLAG_RELOAD, 0);
            if (!hUrl) {
                InternetCloseHandle(hInternet);
                return false;
            }

            std::ofstream out_file(full_path, std::ios::binary);
            if (!out_file.is_open()) {
                InternetCloseHandle(hUrl);
                InternetCloseHandle(hInternet);
                return false;
            }

            char buffer[4096];
            DWORD bytes_read;
            bool success = true;

            while (InternetReadFile(hUrl, buffer, sizeof(buffer), &bytes_read) && bytes_read > 0) {
                out_file.write(buffer, bytes_read);
                if (out_file.fail()) {
                    success = false;
                    break;
                }
            }

            out_file.close();
            InternetCloseHandle(hUrl);
            InternetCloseHandle(hInternet);

            if (!success) {
                std::filesystem::remove(save_path); // 失敗時はファイルを削除
            }

            return success;
        }
#endif // PAXS_PLATFORM_WINDOWS
#endif // PAXS_USING_SFML
    };

} // namespace paxg

#endif // !PAX_GRAPHICA_NETWORK_HPP
