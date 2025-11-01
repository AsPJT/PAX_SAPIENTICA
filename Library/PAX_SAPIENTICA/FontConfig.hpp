/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]      2023-2024 As Project
    [Production]    2023-2024 As Project
    [Contact Us]    wanotaitei@gmail.com            https://github.com/AsPJT/PAX_SAPIENTICA
    [License]       Distributed under the CC0 1.0.  https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_FONT_CONFIG_HPP
#define PAX_SAPIENTICA_FONT_CONFIG_HPP

namespace paxs {

    /// @brief フォント設定の定数
    /// @brief Font configuration constants
    struct FontConfig {
        // プルダウンメニューのフォント設定
        static constexpr int PULLDOWN_FONT_SIZE =
#if defined(PAXS_USING_DXLIB) && defined(__ANDROID__)
            40;
#else
            24;
#endif
        static constexpr int PULLDOWN_FONT_BUFFER_THICKNESS = 3;

        // 暦表示のフォント設定
        static constexpr int KOYOMI_FONT_SIZE = 22;
        static constexpr int KOYOMI_FONT_BUFFER_THICKNESS = 3;
    };

}

#endif // !PAX_SAPIENTICA_FONT_CONFIG_HPP
