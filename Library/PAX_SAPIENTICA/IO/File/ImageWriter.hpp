/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_IO_FILE_IMAGE_WRITER_HPP
#define PAX_SAPIENTICA_IO_FILE_IMAGE_WRITER_HPP

#ifndef PAXS_STB_IMAGE_WRITE_IMPLEMENTED
#define PAXS_STB_IMAGE_WRITE_IMPLEMENTED
#define STB_IMAGE_WRITE_IMPLEMENTATION
#endif
#include <stb/stb_image_write.h>

#include <PAX_SAPIENTICA/System/AppConfig.hpp>
#include <PAX_SAPIENTICA/Utility/Logger.hpp>

namespace paxs {

    /// @brief Handle image file writing operations.
    /// @brief 画像ファイルの書き込み処理を扱う
    struct ImageWriter {

        /// @brief Write PNG image file.
        /// @brief PNGファイルに画像を書き込む。
        /// @param relative_path アセットルートからの相対パス / Relative path from the asset root.
        /// @param width 画像の幅 / Image width.
        /// @param height 画像の高さ / Image height.
        /// @param channels チャンネル数（例：RGBA=4） / Number of channels (e.g., RGBA=4).
        /// @param data 画像データ / Image data.
        /// @return 成功時true / True on success.
        static bool writePngImage(
            const std::string& relative_path,
            int width,
            int height,
            int channels,
            const void* data
        ) noexcept {
            // 相対パスを絶対パスに変換
            const std::string root_path = AppConfig::getInstance().getRootPath();
            const std::string full_path = root_path + relative_path;

            // PNG書き込み（stbi_write_pngは外部ライブラリなので絶対パスが必要）
            const int result = stbi_write_png(full_path.c_str(), width, height, channels, data, 0);

            if (result == 0) {
                PAXS_ERROR("Failed to write PNG file: " + relative_path);
                return false;
            }

            return true;
        }
    };
}

#endif // !PAX_SAPIENTICA_IO_FILE_IMAGE_WRITER_HPP

