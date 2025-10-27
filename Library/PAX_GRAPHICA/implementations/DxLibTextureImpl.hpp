/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_GRAPHICA_DXLIB_TEXTURE_IMPL_HPP
#define PAX_GRAPHICA_DXLIB_TEXTURE_IMPL_HPP

#if defined(PAXS_USING_DXLIB)

#include <DxLib.h>

#include <PAX_GRAPHICA/TextureImpl.hpp>
#include <PAX_GRAPHICA/String.hpp>

namespace paxg {

    class DxLibTextureImpl : public TextureImpl {
    private:
        int texture = -1;
        int cachedWidth = 1;
        int cachedHeight = 1;

    public:
        DxLibTextureImpl() = default;

        DxLibTextureImpl(const String& path) {
            std::string path_str = convertSvgToPng(path.string);
            texture = DxLib::LoadGraph(path_str.c_str());
            DxLib::GetGraphSize(texture, &cachedWidth, &cachedHeight);
        }

        DxLibTextureImpl(const std::string& path) {
            std::string path_str = convertSvgToPng(path);
            texture = DxLib::LoadGraph(path_str.c_str());
            DxLib::GetGraphSize(texture, &cachedWidth, &cachedHeight);
        }

        ~DxLibTextureImpl() {
            if (texture != -1) {
                DxLib::DeleteGraph(texture);
            }
        }

        bool isValid() const override {
            return texture != -1;
        }

        int width() const override {
            return cachedWidth;
        }

        int height() const override {
            return cachedHeight;
        }

        void draw() const override {
            DxLib::DrawGraph(0, 0, texture, TRUE);
        }

        void drawAt(const Vec2f& pos) const override {
            DxLib::DrawGraph(
                static_cast<int>(pos.x() - (cachedWidth / 2.0f)),
                static_cast<int>(pos.y() - (cachedHeight / 2.0f)),
                texture, TRUE);
        }

        void drawAt(const Vec2i& pos) const override {
            DxLib::DrawGraph(
                pos.x() - (cachedWidth / 2),
                pos.y() - (cachedHeight / 2),
                texture, TRUE);
        }

        void resizedDrawAt(const Vec2i& resize, const Vec2i& pos) const override {
            DxLib::DrawExtendGraph(
                pos.x() - (resize.x() / 2), pos.y() - (resize.y() / 2),
                pos.x() + (resize.x() / 2), pos.y() + (resize.y() / 2),
                texture, TRUE);
        }

        void resizedDrawAt(int resize, const Vec2i& pos) const override {
            const int resize_x = resize * cachedWidth / cachedHeight;
            DxLib::DrawExtendGraph(
                pos.x() - (resize_x / 2), pos.y() - (resize / 2),
                pos.x() + (resize_x / 2), pos.y() + (resize / 2),
                texture, TRUE);
        }

        void resizedDrawAt(const Vec2f& resize, const Vec2f& pos) const override {
            DxLib::DrawExtendGraph(
                static_cast<int>(pos.x() - (resize.x() / 2)),
                static_cast<int>(pos.y() - (resize.y() / 2)),
                static_cast<int>(pos.x() + (resize.x() / 2)),
                static_cast<int>(pos.y() + (resize.y() / 2)),
                texture, TRUE);
        }

        void resizedDrawAt(int resize, const Vec2f& pos) const override {
            DxLib::DrawExtendGraph(
                static_cast<int>(pos.x() - (resize / 2.0f)),
                static_cast<int>(pos.y() - (resize / 2.0f)),
                static_cast<int>(pos.x() + (resize / 2.0f)),
                static_cast<int>(pos.y() + (resize / 2.0f)),
                texture, TRUE);
        }

        void resizedDraw(const Vec2i& resize, const Vec2i& pos) const override {
            DxLib::DrawExtendGraph(
                pos.x(), pos.y(),
                pos.x() + resize.x(), pos.y() + resize.y(),
                texture, TRUE);
        }

        void resizedDraw(int resize, const Vec2i& pos) const override {
            DxLib::DrawExtendGraph(
                pos.x(), pos.y(),
                pos.x() + resize, pos.y() + resize,
                texture, TRUE);
        }

        void resizedDraw(const Vec2f& resize, const Vec2f& pos) const override {
            DxLib::DrawExtendGraph(
                static_cast<int>(pos.x()), static_cast<int>(pos.y()),
                static_cast<int>(pos.x() + resize.x()), static_cast<int>(pos.y() + resize.y()),
                texture, TRUE);
        }

        void resizedDraw(int resize, const Vec2f& pos) const override {
            DxLib::DrawExtendGraph(
                static_cast<int>(pos.x()), static_cast<int>(pos.y()),
                static_cast<int>(pos.x() + resize), static_cast<int>(pos.y() + resize),
                texture, TRUE);
        }
    };

} // namespace paxg

#endif // defined(PAXS_USING_DXLIB)

#endif // !PAX_GRAPHICA_DXLIB_TEXTURE_IMPL_HPP
