/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_MAHOROBA_RENDER_LAYER_MANAGER_HPP
#define PAX_MAHOROBA_RENDER_LAYER_MANAGER_HPP

#include <algorithm>
#include <vector>

#include <PAX_MAHOROBA/Rendering/IRenderable.hpp>
#include <PAX_MAHOROBA/Rendering/RenderLayer.hpp>

namespace paxs {

    /// @brief レンダリングレイヤーマネージャー
    /// @brief Rendering layer manager
    ///
    /// IRenderable オブジェクトを登録し、Z順序（レイヤー順）に従って自動的に描画します。
    /// Registers IRenderable objects and automatically renders them in Z-order (layer order).
    class RenderLayerManager {
    private:
        /// @brief 登録された描画可能オブジェクトのリスト
        /// @brief List of registered renderable objects
        std::vector<IRenderable*> renderables_;

        /// @brief ソート済みフラグ（最適化用）
        /// @brief Sorted flag (for optimization)
        bool is_sorted_ = false;

    public:
        /// @brief デフォルトコンストラクタ
        /// @brief Default constructor
        RenderLayerManager() = default;

        /// @brief 描画可能オブジェクトを登録
        /// @brief Register a renderable object
        /// @param renderable 登録するオブジェクト / Object to register
        void registerRenderable(IRenderable* renderable) {
            if (renderable == nullptr) return;
            renderables_.emplace_back(renderable);
            is_sorted_ = false;  // 新規登録があったらソートフラグをリセット
        }

        /// @brief 描画可能オブジェクトの登録を解除
        /// @brief Unregister a renderable object
        /// @param renderable 登録解除するオブジェクト / Object to unregister
        void unregisterRenderable(IRenderable* renderable) {
            auto it = std::find(renderables_.begin(), renderables_.end(), renderable);
            if (it != renderables_.end()) {
                renderables_.erase(it);
                // ソートフラグはそのまま（削除してもソート順は維持される）
            }
        }

        /// @brief すべての登録を解除
        /// @brief Unregister all objects
        void clear() {
            renderables_.clear();
            is_sorted_ = false;
        }

        /// @brief 登録されているオブジェクト数を取得
        /// @brief Get the number of registered objects
        /// @return オブジェクト数 / Number of objects
        std::size_t size() const {
            return renderables_.size();
        }

        /// @brief Z順序に従ってソート
        /// @brief Sort in Z-order
        ///
        /// レイヤー値が小さい順（背景→前面）にソートします。
        /// Sorts in ascending layer value order (background → foreground).
        void sort() {
            std::stable_sort(renderables_.begin(), renderables_.end(),
                [](const IRenderable* a, const IRenderable* b) {
                    return a->getLayer() < b->getLayer();
                });
            is_sorted_ = true;
        }

        /// @brief すべての登録オブジェクトをZ順序に従って描画
        /// @brief Render all registered objects in Z-order
        ///
        /// 非表示のオブジェクトはスキップされます。
        /// Invisible objects are skipped.
        void renderAll() {
            // ソートされていない場合は自動的にソート
            if (!is_sorted_) {
                sort();
            }

            // Z順序（背景→前面）で描画
            for (IRenderable* renderable : renderables_) {
                if (renderable != nullptr && renderable->isVisible()) {
                    renderable->render();
                }
            }
        }

        /// @brief 指定されたレイヤーの範囲内のオブジェクトのみを描画
        /// @brief Render only objects within the specified layer range
        /// @param min_layer 最小レイヤー / Minimum layer (inclusive)
        /// @param max_layer 最大レイヤー / Maximum layer (inclusive)
        void renderRange(RenderLayer min_layer, RenderLayer max_layer) {
            // ソートされていない場合は自動的にソート
            if (!is_sorted_) {
                sort();
            }

            // 指定範囲のレイヤーのみ描画
            for (IRenderable* renderable : renderables_) {
                if (renderable == nullptr || !renderable->isVisible()) continue;

                RenderLayer layer = renderable->getLayer();
                if (layer >= min_layer && layer <= max_layer) {
                    renderable->render();
                }
            }
        }

        /// @brief 特定のレイヤーのオブジェクトのみを描画
        /// @brief Render only objects of a specific layer
        /// @param target_layer 描画するレイヤー / Layer to render
        void renderLayer(RenderLayer target_layer) {
            // ソート不要（特定レイヤーのみフィルタリング）
            for (IRenderable* renderable : renderables_) {
                if (renderable == nullptr || !renderable->isVisible()) continue;

                if (renderable->getLayer() == target_layer) {
                    renderable->render();
                }
            }
        }
    };

} // namespace paxs

#endif // !PAX_MAHOROBA_RENDER_LAYER_MANAGER_HPP
