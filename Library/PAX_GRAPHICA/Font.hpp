/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_GRAPHICA_FONT_HPP
#define PAX_GRAPHICA_FONT_HPP

#include <memory>
#include <string>

#include <PAX_GRAPHICA/Color.hpp>
#include <PAX_GRAPHICA/Interface/FontImpl.hpp>
#include <PAX_SAPIENTICA/Core/Type/Vector2.hpp>

// Include platform-specific implementations
#if defined(PAXS_USING_SIV3D)
    #include <PAX_GRAPHICA/Siv3D/Siv3DFontImpl.hpp>
#elif defined(PAXS_USING_DXLIB)
    #include <PAX_GRAPHICA/DxLib/DxLibFontImpl.hpp>
#elif defined(PAXS_USING_SFML)
    #include <PAX_GRAPHICA/SFML/SFMLFontImpl.hpp>
#else
    #include <PAX_GRAPHICA/Null/NullFontImpl.hpp>
#endif

namespace paxg {

    /// @brief Font wrapper using Pimpl idiom
    /// @brief Pimplイディオムを使用したフォントラッパー
    struct Font {
    private:
        std::shared_ptr<FontImpl> impl;

    public:
        constexpr Font() = default;

        /// @brief Construct a font
        /// @brief フォントを構築
        /// @param size_ Font size
        /// @param path Font file path (empty for default font)
        /// @param buffer_thickness Buffer thickness for rendering
        Font(int size_, const std::string& path = "", int buffer_thickness = 0) {
#if defined(PAXS_USING_SIV3D)
            impl = std::make_shared<Siv3DFontImpl>(size_, path, buffer_thickness);
#elif defined(PAXS_USING_DXLIB)
            impl = std::make_shared<DxLibFontImpl>(size_, path, buffer_thickness);
#elif defined(PAXS_USING_SFML)
            impl = std::make_shared<SFMLFontImpl>(size_, path, buffer_thickness);
#else
            impl = std::make_shared<NullFontImpl>(size_, path, buffer_thickness);
#endif
        }

        /// @brief Set outline effect
        /// @brief アウトライン効果を設定
        void setOutline(double inner, double outer, const Color& color) {
            if (impl) impl->setOutline(inner, outer, color);
        }

        /// @brief Draw text with bottom-left alignment (horizontal: left, vertical: bottom)
        /// @brief テキストを左下揃えで描画 (横:左端, 縦:下端)
        void drawBottomLeft(const std::string& str, const paxs::Vector2<int>& pos, const Color& color) const {
            if (impl) impl->drawBottomLeft(str, pos, color);
        }

        void drawBottomLeft(const std::string& str, const paxs::Vector2<double>& pos, const Color& color) const {
            drawBottomLeft(str, paxs::Vector2<int>{pos}, color);
        }

        /// @brief Draw text with top-right alignment (horizontal: right, vertical: top)
        /// @brief テキストを右上揃えで描画 (横:右端, 縦:上端)
        void drawTopRight(const std::string& str, const paxs::Vector2<int>& pos, const Color& color) const {
            if (impl) impl->drawTopRight(str, pos, color);
        }

        void drawTopRight(const std::string& str, const paxs::Vector2<double>& pos, const Color& color) const {
            drawTopRight(str, paxs::Vector2<int>{pos}, color);
        }

        /// @brief Draw text with bottom-right alignment (horizontal: right, vertical: bottom)
        /// @brief テキストを右下揃えで描画 (横:右端, 縦:下端)
        void drawBottomRight(const std::string& str, const paxs::Vector2<int>& pos, const Color& color) const {
            if (impl) impl->drawBottomRight(str, pos, color);
        }

        void drawBottomRight(const std::string& str, const paxs::Vector2<double>& pos, const Color& color) const {
            drawBottomRight(str, paxs::Vector2<int>{pos}, color);
        }

        /// @brief Draw text with top-left alignment (horizontal: left, vertical: top)
        /// @brief テキストを左上揃えで描画 (横:左端, 縦:上端)
        void draw(const std::string& str, const paxs::Vector2<int>& pos, const Color& color) const {
            if (impl) impl->draw(str, pos, color);
        }

        void draw(const std::string& str, const paxs::Vector2<double>& pos, const Color& color) const {
            draw(str, paxs::Vector2<int>{pos}, color);
        }

        /// @brief Draw text with bottom-center alignment (horizontal: center, vertical: bottom)
        /// @brief テキストを中央下揃えで描画 (横:中央, 縦:下端)
        void drawBottomCenter(const std::string& str, const paxs::Vector2<int>& pos, const Color& color) const {
            if (impl) impl->drawBottomCenter(str, pos, color);
        }

        void drawBottomCenter(const std::string& str, const paxs::Vector2<double>& pos, const Color& color) const {
            drawBottomCenter(str, paxs::Vector2<int>{pos}, color);
        }

        /// @brief Draw text with top-center alignment (horizontal: center, vertical: top)
        /// @brief テキストを中央上揃えで描画 (横:中央, 縦:上端)
        void drawTopCenter(const std::string& str, const paxs::Vector2<int>& pos, const Color& color) const {
            if (impl) impl->drawTopCenter(str, pos, color);
        }

        void drawTopCenter(const std::string& str, const paxs::Vector2<double>& pos, const Color& color) const {
            drawTopCenter(str, paxs::Vector2<int>{pos}, color);
        }

        /// @brief Draw text with center alignment (horizontal: center, vertical: center)
        /// @brief テキストを中央揃えで描画 (横:中央, 縦:中央)
        void drawAt(const std::string& str, const paxs::Vector2<int>& pos, const Color& color) const {
            if (impl) impl->drawAt(str, pos, color);
        }

        void drawAt(const std::string& str, const paxs::Vector2<double>& pos, const Color& color) const {
            drawAt(str, paxs::Vector2<int>{pos}, color);
        }

        /// @brief Get font height
        /// @brief フォントの高さを取得
        int height() const {
            return impl ? impl->height() : 0;
        }

        /// @brief Get text width
        /// @brief テキストの幅を取得
        int width(const std::string& str) {
            return impl ? impl->width(str) : 0;
        }
    };

    /// @brief Font configuration constants
    /// @brief フォント設定の定数
    struct FontConfig {
        // プルダウンメニューのフォント設定
        static constexpr int PULLDOWN_FONT_SIZE =
#if defined(PAXS_USING_DXLIB) && defined(PAXS_PLATFORM_ANDROID)
            34;
#else
            21;
#endif
        static constexpr int PULLDOWN_FONT_BUFFER_THICKNESS = 3;

        // 暦表示のフォント設定
        static constexpr int KOYOMI_FONT_SIZE = 19;
        static constexpr int KOYOMI_FONT_BUFFER_THICKNESS = 3;
    };

} // namespace paxg

#endif // !PAX_GRAPHICA_FONT_HPP
