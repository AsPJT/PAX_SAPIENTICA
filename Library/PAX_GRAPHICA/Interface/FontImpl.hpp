/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_GRAPHICA_FONT_IMPL_HPP
#define PAX_GRAPHICA_FONT_IMPL_HPP

#include <string>

#include <PAX_GRAPHICA/Color.hpp>

#include <PAX_SAPIENTICA/Core/Type/Vector2.hpp>

namespace paxg {

    /// @brief Abstract base class for font implementations
    /// @brief フォント実装の抽象基底クラス
    class FontImpl {
    public:
        virtual ~FontImpl() = default;

        /// @brief Set outline effect
        /// @brief アウトライン効果を設定
        virtual void setOutline(double inner, double outer, const Color& color) = 0;

        /// @brief Draw text with bottom-left alignment (horizontal: left, vertical: bottom)
        /// @brief テキストを左下揃えで描画 (横:左端, 縦:下端)
        virtual void drawBottomLeft(const std::string& str, const paxs::Vector2<int>& pos, const Color& color) const = 0;

        /// @brief Draw text with top-right alignment (horizontal: right, vertical: top)
        /// @brief テキストを右上揃えで描画 (横:右端, 縦:上端)
        virtual void drawTopRight(const std::string& str, const paxs::Vector2<int>& pos, const Color& color) const = 0;

        /// @brief Draw text with bottom-right alignment (horizontal: right, vertical: bottom)
        /// @brief テキストを右下揃えで描画 (横:右端, 縦:下端)
        virtual void drawBottomRight(const std::string& str, const paxs::Vector2<int>& pos, const Color& color) const = 0;

        /// @brief Draw text with top-left alignment (horizontal: left, vertical: top)
        /// @brief テキストを左上揃えで描画 (横:左端, 縦:上端)
        virtual void draw(const std::string& str, const paxs::Vector2<int>& pos, const Color& color) const = 0;

        /// @brief Draw text with bottom-center alignment (horizontal: center, vertical: bottom)
        /// @brief テキストを中央下揃えで描画 (横:中央, 縦:下端)
        virtual void drawBottomCenter(const std::string& str, const paxs::Vector2<int>& pos, const Color& color) const = 0;

        /// @brief Draw text with top-center alignment (horizontal: center, vertical: top)
        /// @brief テキストを中央上揃えで描画 (横:中央, 縦:上端)
        virtual void drawTopCenter(const std::string& str, const paxs::Vector2<int>& pos, const Color& color) const = 0;

        /// @brief Draw text with center alignment (horizontal: center, vertical: center)
        /// @brief テキストを中央揃えで描画 (横:中央, 縦:中央)
        virtual void drawAt(const std::string& str, const paxs::Vector2<int>& pos, const Color& color) const = 0;

        /// @brief Get font height
        /// @brief フォントの高さを取得
        virtual int height() const = 0;

        /// @brief Get text width
        /// @brief テキストの幅を取得
        virtual int width(const std::string& str) = 0;
    };

} // namespace paxg

#endif // !PAX_GRAPHICA_FONT_IMPL_HPP
