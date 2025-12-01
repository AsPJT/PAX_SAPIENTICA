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
#include <PAX_MAHOROBA/UI/Debug/PerformanceScope.hpp>
#include <PAX_MAHOROBA/UI/LoadingProgressBar.hpp>

#include <PAX_SAPIENTICA/Interface/IProgressReporter.hpp>
#include <PAX_SAPIENTICA/System/Async/LoadingHandle.hpp>
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

        event_bus_ = &EventBus::getInstance();
        app_state_ = std::make_unique<AppStateManager>();

        // フォントシステムを初期化（進捗バーの描画に必要）
        Fonts().initialize();

        // ローディング画面の画像を読み込んで保持
        loading_texture_ = std::make_unique<paxg::Texture>("Data/LoadingScreen/LoadingScreen.png");

        // 初期ウィンドウサイズを記録
        last_window_width_ = paxg::Window::width();
        last_window_height_ = paxg::Window::height();

        // 初期化用の進捗バーを作成
        init_progress_bar_ = std::make_unique<LoadingProgressBar<bool>>(
            &init_loading_handle_,
            (last_window_width_ / 2) - 200,
            (last_window_height_ / 2) - 15,
            400,
            30,
            Fonts().getFont(FontProfiles::ui_medium)
        );

        // 非同期初期化を開始
        init_loading_handle_ = startAsyncTask<bool>([this](ProgressToken<bool>& token) {
            // Localesシステムを初期化（時間がかかる処理）
            // 進捗範囲: 0.05 ～ 0.65
            paxs::CallbackProgressReporter locales_reporter([&token](float progress, const std::string& message) {
                token.setProgress(progress);
                token.setMessage(message);
            });
            Fonts().initializeLocales(&locales_reporter, 0.05f, 0.65f);

            // ApplicationEventHandler作成（イベント購読管理）
            token.setProgress(0.65f);
            token.setMessage("Creating ApplicationEventHandler...");
            this->event_handler_ = std::make_unique<ApplicationEventHandler>(*this->app_state_);

            // InputManager作成（入力処理統合）
            token.setProgress(0.7f);
            token.setMessage("Creating InputManager...");
            this->input_manager_ = std::make_unique<InputManager>();

            // AppComponentManager作成（コンポーネント統合管理）
            token.setProgress(0.75f);
            token.setMessage("Creating AppComponentManager...");
            this->component_manager_ = std::make_unique<AppComponentManager>(*this->app_state_);

            // 入力ハンドラー作成と登録
            token.setProgress(0.8f);
            token.setMessage("Creating input handlers...");
            this->map_viewport_input_handler_ = std::make_unique<MapViewportInputHandler>(
                this->app_state_->getMapViewportForInputHandler()
            );
            this->ui_input_handler_ = std::make_unique<UIInputHandler>();

#ifdef PAXS_DEVELOPMENT
            this->debug_input_handler_ = std::make_unique<DebugInputHandler>(
                &this->component_manager_->getDebugLayer()
            );
#endif

            // InputManagerに入力ハンドラーを登録
            token.setProgress(0.85f);
            token.setMessage("Registering input handlers...");
            this->input_manager_->registerHandler(this->map_viewport_input_handler_.get());
            this->input_manager_->registerHandler(this->ui_input_handler_.get());
#ifdef PAXS_DEVELOPMENT
            this->input_manager_->registerHandler(this->debug_input_handler_.get());
#endif

            // AppComponentManagerが内部のウィジェット/ハンドラーを登録
            token.setProgress(0.9f);
            token.setMessage("Registering widgets...");
            this->component_manager_->registerToInputHandlers(
                *this->ui_input_handler_,
                this->input_manager_->getInputRouter()
            );

#ifdef PAXS_DEVELOPMENT
            // デバッグコンソールにカスタムコマンドを登録
            token.setProgress(0.95f);
            token.setMessage("Registering debug commands...");
            DebugConsoleCommandRegistry::registerAllCommands(
                this->component_manager_->getDebugLayer().getConsole(),
                *this->app_state_
            );
#endif

            token.setProgress(1.0f);
            token.setMessage("Initialization complete!");
            return true;
        });
    }

    /// @brief メインループを実行
    void run() {
        while (paxg::Window::update()) {
            PERF_BEGIN_FRAME();
            PERF_SCOPE("MainLoop");

            // 画像の拡大縮小の方式を設定
            const paxg::ScopedSamplerState sampler{ paxg::SamplerState::ClampNearest };

            // アプリケーション状態に応じて処理を分岐
            if (app_state_ && app_state_->getAppState() == AppState::Initializing) {
                PERF_SCOPE("Initializing");
                updateInitializingMode();
            }
#ifdef PAXS_USING_SIMULATOR
            else if (app_state_ && app_state_->getAppState() == AppState::Loading) {
                PERF_SCOPE("Loading");
                updateLoadingMode();
            }
#endif
            else {
                PERF_SCOPE("Running");
                updateRunningMode();
            }

            PERF_END_FRAME();
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

    // 初期化用
    LoadingHandle<bool> init_loading_handle_;
    std::unique_ptr<LoadingProgressBar<bool>> init_progress_bar_;
    std::unique_ptr<paxg::Texture> loading_texture_;  ///< ローディング画面の画像 / Loading screen texture
    int last_window_width_ = 0;   ///< 前回のウィンドウ幅 / Last window width
    int last_window_height_ = 0;  ///< 前回のウィンドウ高さ / Last window height

    /// @brief 初期化中の更新処理
    void updateInitializingMode() {
        // 1. 描画処理を先に実行（ローディング画像 + 進捗バー）
        if (loading_texture_) {
            // 画面サイズを取得
            const int window_width = paxg::Window::width();
            const int window_height = paxg::Window::height();

            // ウィンドウサイズが変更された場合、進捗バーを再作成
            if (init_progress_bar_ && (window_width != last_window_width_ || window_height != last_window_height_)) {
                last_window_width_ = window_width;
                last_window_height_ = window_height;
                init_progress_bar_ = std::make_unique<LoadingProgressBar<bool>>(
                    &init_loading_handle_,
                    (window_width / 2) - 200,
                    (window_height / 2) - 15,
                    400,
                    30,
                    Fonts().getFont(FontProfiles::ui_medium)
                );
            }

            // 画像のサイズを取得
            const float texture_width = static_cast<float>(loading_texture_->width());
            const float texture_height = static_cast<float>(loading_texture_->height());

            // 画像をウィンドウサイズに合わせてスケーリング
            // アスペクト比を維持しながらウィンドウ全体を覆うようにする（大きい方のスケールを使用）
            const float scale_x = static_cast<float>(window_width) / texture_width;
            const float scale_y = static_cast<float>(window_height) / texture_height;
            const float scale = (scale_x > scale_y) ? scale_x : scale_y;

            const int scaled_width = static_cast<int>(texture_width * scale);
            const int scaled_height = static_cast<int>(texture_height * scale);

            // 中央配置の座標を計算
            const int pos_x = (window_width - scaled_width) / 2;
            const int pos_y = (window_height - scaled_height) / 2;

            // ローディング画像をリサイズして描画
            loading_texture_->resizedDraw(paxg::Vec2i{scaled_width, scaled_height}, paxg::Vec2i{pos_x, pos_y});
        }

        // 進捗バーを描画
        if (init_progress_bar_) {
            init_progress_bar_->render();
        }

        // 2. ロード状態の更新（ロード完了のチェック）
        // 描画後にチェックすることで、最後のフレームが確実に表示される
        if (init_loading_handle_.isValid() && init_loading_handle_.isFinished()) {
            const bool* result = init_loading_handle_.getResult();

            if ((result != nullptr) && *result) {
                // 初期化成功 - アプリケーション状態をRunningに変更
                app_state_->updateInitializationComplete();

                // ローディング画面終了
                paxs::PaxSapienticaInit::endLoadingScreen();

                // 進捗バーとローディング画像をクリア
                init_progress_bar_.reset();
                loading_texture_.reset();
            } else {
                // 初期化失敗
                if (init_loading_handle_.hasError()) {
                    PAXS_ERROR("Application initialization failed");
                }
            }
        }

        // 3. タッチ入力の状態更新
        paxg::TouchInput::updateState();
    }

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
        {
            PERF_SCOPE("InputProcessing");
            input_manager_->processInput();
        }

        // 2. イベントキュー処理（遅延イベントの処理）
        {
            PERF_SCOPE("EventQueue");
            event_bus_->processQueue();
        }

#ifdef PAXS_USING_SIMULATOR
        // 3. 暦更新（暦再生時の時間進行）
        {
            PERF_SCOPE("KoyomiUpdate");
            app_state_->updateKoyomi();
        }
#endif

#ifdef PAXS_DEVELOPMENT
        // 4. デバッグレイヤーの更新（自動削除チェック）
        {
            PERF_SCOPE("DebugLayerUpdate");
            component_manager_->getDebugLayer().update(
                component_manager_->getDebugLayer().getCurrentTime()
            );
        }
#endif

        // 5. 描画処理
        {
            PERF_SCOPE("Rendering");
            component_manager_->render();
        }

        // 6. タッチ入力の状態更新
        {
            PERF_SCOPE("TouchInputUpdate");
            paxg::TouchInput::updateState();
        }
    }
};

} // namespace paxs

#endif // !PAX_MAHOROBA_CORE_APPLICATION_HPP
