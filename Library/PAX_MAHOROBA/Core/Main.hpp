/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

//#define PAXS_DEVELOPMENT
#ifndef PAX_MAHOROBA_MAIN_HPP
#define PAX_MAHOROBA_MAIN_HPP

#if defined(PAXS_USING_DXLIB) && defined(__ANDROID__)
// シミュレーションを使用しない
#else
#define PAXS_USING_SIMULATOR
#endif

#include <memory>

#include <PAX_GRAPHICA/TouchInput.hpp>

#include <PAX_MAHOROBA/Core/AppComponentManager.hpp>
#include <PAX_MAHOROBA/Core/AppStateManager.hpp>
#include <PAX_MAHOROBA/Core/EventBus.hpp>
#include <PAX_MAHOROBA/Core/InitLogo.hpp>
#include <PAX_MAHOROBA/Input/InputManager.hpp>
#include <PAX_MAHOROBA/Input/MapViewportInputHandler.hpp>
#include <PAX_MAHOROBA/Input/UIInputHandler.hpp>
#include <PAX_MAHOROBA/Rendering/FontSystem.hpp>

namespace paxs {

    void startMain() {
        // 初期化とロゴの表示
        paxs::PaxSapienticaInit::firstInit();

        // フォントシステムを初期化
        Fonts().initialize();

        // 1. EventBus作成（シングルトン）
        EventBus& event_bus = EventBus::getInstance();

        // 2. AppStateManager作成（ドメインロジック集約）
        AppStateManager app_state(event_bus);

        // 3. InputManager作成（入力処理統合）
        InputManager input_manager(event_bus);

        // 4. AppComponentManager作成（コンポーネント統合管理）
        AppComponentManager component_manager(event_bus, app_state);

        // 5. 入力ハンドラー登録
        std::unique_ptr<MapViewportInputHandler> map_viewport_input_handler =
            std::make_unique<MapViewportInputHandler>();
        map_viewport_input_handler->setViewport(&app_state.getMapViewport());

        std::unique_ptr<UIInputHandler> ui_input_handler =
            std::make_unique<UIInputHandler>();

        // InputManagerに入力ハンドラーを登録
        input_manager.registerHandler(map_viewport_input_handler.get());
        input_manager.registerHandler(ui_input_handler.get());

        // AppComponentManagerが内部のウィジェット/ハンドラーを登録
        component_manager.registerToInputHandlers(*ui_input_handler, input_manager.getInputRouter());

        // ローディング画面終了
        paxs::PaxSapienticaInit::endLoadingScreen();

        /*##########################################################################################
            ループ開始
        ##########################################################################################*/
        while (paxg::Window::update()) {
            // 画像の拡大縮小の方式を設定
            const paxg::ScopedSamplerState sampler{ paxg::SamplerState::ClampNearest };

            // 1. 入力処理（イベント収集・ルーティング）
            input_manager.processInput();

            // 2. イベントキュー処理（遅延イベントの処理）
            event_bus.processQueue();

            // 3. 暦更新（シミュレーション実行中の場合）
            app_state.updateKoyomi();

            // 4. タイルデータ更新（ビューポート変更に応じてタイルをロード）
            component_manager.updateTiles();

            // 5. 描画のみ（更新は不要、データはAppStateManagerから直接取得）
            component_manager.render();

            // 6. タッチ入力の状態更新
            paxg::TouchInput::updateState();
        }
    }
}

#endif // !PAX_MAHOROBA_MAIN_HPP
