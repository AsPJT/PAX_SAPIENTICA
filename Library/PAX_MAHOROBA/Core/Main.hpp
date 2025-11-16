/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

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
#include <PAX_MAHOROBA/Core/InitLogo.hpp>
#include <PAX_MAHOROBA/Input/InputManager.hpp>
#include <PAX_MAHOROBA/Input/MapViewportInputHandler.hpp>
#include <PAX_MAHOROBA/Input/UIInputHandler.hpp>
#include <PAX_MAHOROBA/Rendering/FontSystem.hpp>

#include <PAX_SAPIENTICA/System/EventBus.hpp>

#ifdef PAXS_DEVELOPMENT
#include <PAX_MAHOROBA/Input/DebugInputHandler.hpp>
#include <PAX_MAHOROBA/UI/Debug/DebugConsoleCommandRegistry.hpp>
#include <PAX_SAPIENTICA/Utility/LoggerIntegration.hpp>
#endif

namespace paxs {

    void startMain() {
        // 初期化とロゴの表示
        paxs::PaxSapienticaInit::firstInit();

        // フォントシステムを初期化
        Fonts().initialize();

        // 1. EventBus作成（シングルトン）
        EventBus& event_bus = EventBus::getInstance();

        // 2. AppStateManager作成（ドメインロジック集約）
        AppStateManager app_state{};

        // 3. InputManager作成（入力処理統合）
        InputManager input_manager{};

        // 4. AppComponentManager作成（コンポーネント統合管理）
        AppComponentManager component_manager(app_state);

        // 5. 入力ハンドラー登録
        std::unique_ptr<MapViewportInputHandler> map_viewport_input_handler =
            std::make_unique<MapViewportInputHandler>(app_state.getMapViewport());

        std::unique_ptr<UIInputHandler> ui_input_handler =
            std::make_unique<UIInputHandler>();

#ifdef PAXS_DEVELOPMENT
        std::unique_ptr<DebugInputHandler> debug_input_handler =
            std::make_unique<DebugInputHandler>(&component_manager.getDebugLayer());
#endif

        // InputManagerに入力ハンドラーを登録
        input_manager.registerHandler(map_viewport_input_handler.get());
        input_manager.registerHandler(ui_input_handler.get());
#ifdef PAXS_DEVELOPMENT
        input_manager.registerHandler(debug_input_handler.get());
#endif

        // AppComponentManagerが内部のウィジェット/ハンドラーを登録
        component_manager.registerToInputHandlers(*ui_input_handler, input_manager.getInputRouter());

#ifdef PAXS_DEVELOPMENT
        // デバッグコンソールにカスタムコマンドを登録
        DebugConsoleCommandRegistry::registerAllCommands(
            component_manager.getDebugLayer().getConsole(),
            app_state
        );
#endif

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

            // 3. 暦更新（暦再生時の時間進行）
            app_state.updateKoyomi();

#ifdef PAXS_DEVELOPMENT
            // 4. デバッグレイヤーの更新（自動削除チェック）
            component_manager.getDebugLayer().update(
                component_manager.getDebugLayer().getCurrentTime()
            );
#endif

            // 5. 描画処理
            component_manager.render();

            // 6. タッチ入力の状態更新
            paxg::TouchInput::updateState();
        }
    }
}

#endif // !PAX_MAHOROBA_MAIN_HPP
