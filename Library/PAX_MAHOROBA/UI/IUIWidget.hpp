/*##########################################################################################

	PAX SAPIENTICA Library 💀🌿🌏

	[Planning]		2023-2024 As Project
	[Production]	2023-2024 As Project
	[Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
	[License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_MAHOROBA_I_UI_WIDGET_HPP
#define PAX_MAHOROBA_I_UI_WIDGET_HPP

#include <PAX_GRAPHICA/Rect.hpp>
#include <PAX_GRAPHICA/Vec2.hpp>

#include <PAX_MAHOROBA/Input/IInputHandler.hpp>
#include <PAX_MAHOROBA/Rendering/IRenderable.hpp>

#include <PAX_SAPIENTICA/InputStateManager.hpp>

namespace paxs {

    /// @brief UIウィジェットの共通インターフェース
    /// @brief Common interface for UI widgets
    ///
    /// IRenderable と IInputHandler を継承し、レイヤーベースシステムに対応します。
    /// Inherits IRenderable and IInputHandler to support layer-based system.
    class IUIWidget : public IRenderable, public IInputHandler {
    public:
        virtual ~IUIWidget() = default;

        // 基本操作（既存のインターフェース - 後方互換性のため維持）
        // Basic operations (legacy interface - kept for backward compatibility)
        virtual void update(InputStateManager& input_state_manager) = 0;
        virtual void draw() = 0;

        // 位置・サイズ管理
        virtual paxg::Rect getRect() const = 0;
        virtual void setPos(const paxg::Vec2i& pos) = 0;

        // 可視性管理
        void setVisible(bool visible) override = 0;
        bool isVisible() const override = 0;

        // 有効/無効管理
        virtual void setEnabled(bool enabled) = 0;
        bool isEnabled() const override = 0;

        // コンポーネント情報
        virtual const char* getName() const = 0;
        virtual bool isAvailable() const = 0;

        // IRenderable の実装（デフォルト実装を提供）
        // IRenderable implementation (provides default implementation)

        /// @brief レンダリング処理（既存のdraw()を呼び出す）
        /// @brief Render (calls existing draw())
        void render() override {
            draw();
        }

        // getLayer() は各サブクラスで実装必須
        // getLayer() must be implemented by each subclass
        // setVisible(), isVisible() は既に定義済み

        // IInputHandler の実装（デフォルト実装を提供）
        // IInputHandler implementation (provides default implementation)

        /// @brief 入力処理（既存のupdate()を呼び出す）
        /// @brief Handle input (calls existing update())
        /// @param event 入力イベント / Input event
        /// @return 処理した場合true / true if handled
        bool handleInput(const InputEvent& event) override {
            if (event.input_state_manager != nullptr) {
                update(*event.input_state_manager);
                return true; // UIウィジェットは常に処理済みとする
            }
            return false;
        }

        /// @brief ヒットテスト（getRect()を使用）
        /// @brief Hit test (uses getRect())
        /// @param x X座標 / X coordinate
        /// @param y Y座標 / Y coordinate
        /// @return 範囲内ならtrue / true if within bounds
        bool hitTest(int x, int y) const override {
            if (!isVisible() || !isEnabled()) return false;
            const paxg::Rect rect = getRect();
            return (x >= rect.x() && x < rect.x() + rect.w() &&
                    y >= rect.y() && y < rect.y() + rect.h());
        }

        // getLayer() は各サブクラスで実装
        // getLayer() is implemented by subclasses
    };

}

#endif // !PAX_MAHOROBA_I_UI_WIDGET_HPP
