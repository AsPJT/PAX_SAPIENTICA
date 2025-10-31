/*##########################################################################################

	PAX SAPIENTICA Library 💀🌿🌏

	[Planning]		2023-2024 As Project
	[Production]	2023-2024 As Project
	[Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
	[License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_MAHOROBA_FONT_MANAGER_HPP
#define PAX_MAHOROBA_FONT_MANAGER_HPP

#include <array>

#include <PAX_GRAPHICA/Font.hpp>

#include <PAX_MAHOROBA/Rendering/LanguageFonts.hpp>

#include <PAX_SAPIENTICA/FontConfig.hpp>
#include <PAX_SAPIENTICA/Language.hpp>
#include <PAX_SAPIENTICA/Key/LanguageKeys.hpp>

namespace paxs {

    // 文字表示（フォント描画）のみを担当するクラス
    class FontManager {
    public:
        LanguageFonts language_fonts;

        static constexpr std::array path_list = {
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

        void init(const SelectLanguage& select_language) {
            language_fonts.setDefaultPath("Data/Font/noto-sans-sc/NotoSansSC-Regular.otf");
            setLanguageFont(FontConfig::PULLDOWN_FONT_SIZE, FontConfig::PULLDOWN_FONT_BUFFER_THICKNESS);

            pin_font = paxg::Font{ 14, "Data/Font/noto-sans-jp/NotoSansJP-Regular.otf", 2 };
            en_font = paxg::Font{ 20, "Data/Font/noto-sans-jp/NotoSansJP-Regular.otf", 2 };
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
            const int buffer_thickness) {
            for (std::size_t i = 0; i < path_list.size(); ++i) {
                language_fonts.add(
                    path_list[i],
                    paxs::LanguageKeys::ALL_LANGUAGE_HASHES[i],
                    static_cast<std::uint_least8_t>(font_size_),
                    static_cast<std::uint_least8_t>(buffer_thickness)
                );
            }
        }
    };

}

#endif // !PAX_MAHOROBA_FONT_MANAGER_HPP
