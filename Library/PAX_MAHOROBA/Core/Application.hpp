/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_MAHOROBA_CORE_APPLICATION_HPP
#define PAX_MAHOROBA_CORE_APPLICATION_HPP

#include <memory>

#include <PAX_GRAPHICA/TouchInput.hpp>
#include <PAX_GRAPHICA/Window.hpp>

#include <PAX_MAHOROBA/Core/AppComponentManager.hpp>
#include <PAX_MAHOROBA/Core/ApplicationEventHandler.hpp>
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

/// @brief アプリケーションクラス
class Application {
public:
    Application() = default;

    /// @brief アプリケーションを初期化
    void initialize() {
        // 初期化とロゴの表示
        paxs::PaxSapienticaInit::firstInit();

        // フォントシステムを初期化
        Fonts().initialize();

        // 1. EventBus作成（シングルトン）
        event_bus_ = &EventBus::getInstance();

        // 2. AppStateManager作成（ドメインロジック集約）
        app_state_ = std::make_unique<AppStateManager>();

        // 3. ApplicationEventHandler作成（イベント購読管理）
        event_handler_ = std::make_unique<ApplicationEventHandler>(*app_state_);

        // 4. InputManager作成（入力処理統合）
        input_manager_ = std::make_unique<InputManager>();

        // 5. AppComponentManager作成（コンポーネント統合管理）
        component_manager_ = std::make_unique<AppComponentManager>(*app_state_);

        // 6. 入力ハンドラー作成と登録
        map_viewport_input_handler_ = std::make_unique<MapViewportInputHandler>(
            app_state_->getMapViewportForInputHandler()
        );
        ui_input_handler_ = std::make_unique<UIInputHandler>();

#ifdef PAXS_DEVELOPMENT
        debug_input_handler_ = std::make_unique<DebugInputHandler>(
            &component_manager_->getDebugLayer()
        );
#endif

        // InputManagerに入力ハンドラーを登録
        input_manager_->registerHandler(map_viewport_input_handler_.get());
        input_manager_->registerHandler(ui_input_handler_.get());
#ifdef PAXS_DEVELOPMENT
        input_manager_->registerHandler(debug_input_handler_.get());
#endif

        // AppComponentManagerが内部のウィジェット/ハンドラーを登録
        component_manager_->registerToInputHandlers(
            *ui_input_handler_,
            input_manager_->getInputRouter()
        );

#ifdef PAXS_DEVELOPMENT
        // デバッグコンソールにカスタムコマンドを登録
        DebugConsoleCommandRegistry::registerAllCommands(
            component_manager_->getDebugLayer().getConsole(),
            *app_state_
        );
#endif

        // ローディング画面終了
        paxs::PaxSapienticaInit::endLoadingScreen();
    }

    /// @brief メインループを実行
    void run() {
        while (paxg::Window::update()) {
            // 画像の拡大縮小の方式を設定
            const paxg::ScopedSamplerState sampler{ paxg::SamplerState::ClampNearest };

#ifdef PAXS_USING_SIMULATOR
            // アプリケーション状態に応じて処理を分岐
            if (app_state_->getAppState() == AppState::Loading) {
                updateLoadingMode();
            } else
#endif
            {
                updateRunningMode();
            }
        }
    }

private:
    // コアコンポーネント
    EventBus* event_bus_ = nullptr;
    std::unique_ptr<AppStateManager> app_state_;
    std::unique_ptr<ApplicationEventHandler> event_handler_;
    std::unique_ptr<InputManager> input_manager_;
    std::unique_ptr<AppComponentManager> component_manager_;

    // 入力ハンドラー
    std::unique_ptr<MapViewportInputHandler> map_viewport_input_handler_;
    std::unique_ptr<UIInputHandler> ui_input_handler_;
#ifdef PAXS_DEVELOPMENT
    std::unique_ptr<DebugInputHandler> debug_input_handler_;
#endif

#ifdef PAXS_USING_SIMULATOR
    /// @brief ロード中の更新処理
    void updateLoadingMode() {
        // 1. ウィンドウリサイズ検知（UIの再配置のため）
        input_manager_->detectWindowResize();

        // 2. ロード状態の更新（ロード完了のチェック）
        app_state_->updateLoadingState();

        // 3. イベントキュー処理（最小限）
        event_bus_->processQueue();

        // 4. 描画処理（タイルとメニューバーのみ）
        component_manager_->renderLoadingMode();

        // 5. タッチ入力の状態更新
        paxg::TouchInput::updateState();
    }
#endif

    /// @brief 通常実行中の更新処理
    void updateRunningMode() {
        // 1. 入力処理（イベント収集・ルーティング）
        input_manager_->processInput();

        // 2. イベントキュー処理（遅延イベントの処理）
        event_bus_->processQueue();

#ifdef PAXS_USING_SIMULATOR
        // 3. 暦更新（暦再生時の時間進行）
        app_state_->updateKoyomi();
#endif

#ifdef PAXS_DEVELOPMENT
        // 4. デバッグレイヤーの更新（自動削除チェック）
        component_manager_->getDebugLayer().update(
            component_manager_->getDebugLayer().getCurrentTime()
        );
#endif

        // 5. 描画処理
        component_manager_->render();

        // 6. タッチ入力の状態更新
        paxg::TouchInput::updateState();
    }
};

} // namespace paxs

#endif // !PAX_MAHOROBA_CORE_APPLICATION_HPP
