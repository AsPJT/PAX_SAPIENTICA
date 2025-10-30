/*##########################################################################################

	PAX SAPIENTICA Library 💀🌿🌏

	[Planning]		2023-2024 As Project
	[Production]	2023-2024 As Project
	[Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
	[License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_MAHOROBA_STRING_VIEWER_HPP
#define PAX_MAHOROBA_STRING_VIEWER_HPP

#include <string>
#include <vector>

#include <PAX_GRAPHICA/Font.hpp>

#include <PAX_MAHOROBA/LanguageFonts.hpp>

#include <PAX_SAPIENTICA/AppConfig.hpp>
#include <PAX_SAPIENTICA/Language.hpp>
#include <PAX_SAPIENTICA/Key/LanguageKeys.hpp>

namespace paxs {

    // 文字表示（フォント描画）のみを担当するクラス
    class StringViewer {
    public:
        LanguageFonts language_fonts;

        std::vector<std::string> path_list = {
            "Data/Font/noto-sans-jp/NotoSansJP-Regular.otf",
            "Data/Font/noto-sans-jp/NotoSansJP-Regular.otf",
            "Data/Font/noto-sans-sc/NotoSansSC-Regular.otf",
            "Data/Font/noto-sans-sc/NotoSansSC-Regular.otf",
            "Data/Font/noto-sans-kr/NotoSansKR-Regular.otf",
            "Data/Font/noto-sans-jp/NotoSansJP-Regular.otf",
            "Data/Font/noto-sans-jp/NotoSansJP-Regular.otf",
            "Data/Font/noto-sans-jp/NotoSansJP-Regular.otf",
            "Data/Font/noto-sans-jp/NotoSansJP-Regular.otf",
            "Data/Font/noto-sans-jp/NotoSansJP-Regular.otf",
            "Data/Font/noto-sans-jp/NotoSansJP-Regular.otf",
            "Data/Font/noto-sans/NotoSans-Regular.ttf",
            "Data/Font/noto-sans/NotoSans-Regular.ttf",
            "Data/Font/noto-sans-jp/NotoSansJP-Regular.otf",
            "Data/Font/noto-sans-jp/NotoSansJP-Regular.otf",
            "Data/Font/noto-sans/NotoSans-Regular.ttf",
            "Data/Font/noto-sans/NotoSans-Regular.ttf",
            "Data/Font/noto-sans-jp/NotoSansJP-Regular.otf",
            "Data/Font/noto-sans-jp/NotoSansJP-Regular.otf",
            "Data/Font/noto-sans-ar/NotoNaskhArabic-VariableFont_wght.ttf",
            "Data/Font/noto-sans-ar/NotoNaskhArabic-VariableFont_wght.ttf",
            "Data/Font/noto-sans-jp/NotoSansJP-Regular.otf"
        };

        paxg::Font pin_font{};
        paxg::Font en_font{};

        void init(const SelectLanguage& select_language, int pulldown_font_size, int pulldown_font_buffer_thickness_size) {
            language_fonts.setDefaultPath("Data/Font/noto-sans-sc/NotoSansSC-Regular.otf");
            setLanguageFont(pulldown_font_size, AppConfig::getInstance()->getRootPath(), pulldown_font_buffer_thickness_size);

            const std::string path = (AppConfig::getInstance()->getRootPath());

            pin_font = paxg::Font{ 14, (path + "Data/Font/noto-sans-jp/NotoSansJP-Regular.otf"), 2 };
            en_font = paxg::Font{ 20, (path + "Data/Font/noto-sans-jp/NotoSansJP-Regular.otf"), 2 };
        }

        // 言語ごとのフォントを取得・追加
        paxg::Font* getFont(std::uint_least32_t language_key, std::uint_least8_t font_size, std::uint_least8_t buffer_thickness) {
            return language_fonts.getAndAdd(language_key, font_size, buffer_thickness);
        }

        // LanguageFontsへの直接アクセス
        LanguageFonts& getLanguageFonts() {
            return language_fonts;
        }

    private:
        void setLanguageFont(
            const int font_size_,
            const std::string& path,
            const int buffer_thickness) {
            for (std::size_t i = 0; i < path_list.size(); ++i) {
                language_fonts.add(
                    (path + path_list[i]),
                    paxs::LanguageKeys::ALL_LANGUAGE_HASHES[i],
                    static_cast<std::uint_least8_t>(font_size_),
                    static_cast<std::uint_least8_t>(buffer_thickness)
                );
            }
        }
    };

}

#endif // !PAX_MAHOROBA_STRING_VIEWER_HPP
