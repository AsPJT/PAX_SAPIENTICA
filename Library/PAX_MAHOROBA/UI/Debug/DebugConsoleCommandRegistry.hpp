/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_MAHOROBA_UI_DEBUG_DEBUG_CONSOLE_COMMAND_REGISTRY_HPP
#define PAX_MAHOROBA_UI_DEBUG_DEBUG_CONSOLE_COMMAND_REGISTRY_HPP

#include <PAX_MAHOROBA/Core/AppStateManager.hpp>
#include <PAX_MAHOROBA/Map/MapViewport.hpp>
#include <PAX_MAHOROBA/UI/Debug/DebugConsole.hpp>

#include <PAX_SAPIENTICA/Core/Utility/StringUtils.hpp>
#include <PAX_SAPIENTICA/Geography/Coordinate/Projection.hpp>
#include <PAX_SAPIENTICA/System/ApplicationEvents.hpp>
#include <PAX_SAPIENTICA/System/EventBus.hpp>
#include <PAX_SAPIENTICA/Utility/Logger.hpp>

namespace paxs {

#ifdef PAXS_DEVELOPMENT
/// @brief デバッグコンソールコマンドレジストリ
/// @brief Debug console command registry
/// @details アプリケーション固有のコマンドを登録する
class DebugConsoleCommandRegistry {
public:
    /// @brief 全てのコマンドを登録
    /// @brief Register all commands
    /// @param console デバッグコンソール / Debug console
    /// @param app_state アプリケーション状態マネージャー / Application state manager
    static void registerAllCommands(DebugConsole& console, AppStateManager& app_state) {
        registerMapCommands(console, app_state);
#ifdef PAXS_USING_SIMULATOR
        registerSimulationCommands(console, app_state);
#endif
    }

private:
    /// @brief マップ関連コマンドを登録
    /// @brief Register map-related commands
    static void registerMapCommands(DebugConsole& console, AppStateManager& app_state) {
        // x <longitude>: 経度を設定（範囲: 0.0～180.0）
        console.registerCommand("x", [&app_state](const std::vector<std::string>& args) {
            if (args.size() < 2) {
                PAXS_WARNING("Usage: x <longitude> (range: 0.0-180.0)");
                return;
            }
            auto longitude_opt = StringUtils::toDouble(args[1]);
            if (!longitude_opt) {
                PAXS_ERROR("Invalid longitude value: \"" + args[1] + "\"");
                return;
            }
            double longitude = *longitude_opt;
            if (longitude < 0.0 || longitude > 180.0) {
                PAXS_WARNING("Longitude must be between 0.0 and 180.0");
                return;
            }
            // メルカトル座標に変換
            paxs::Vector2<double> equirect_coords(longitude, app_state.getMapViewport().getCenterY());
            paxg::Coordinate mercator_coords = paxs::MercatorDeg(paxs::EquirectangularDeg(equirect_coords));
            app_state.getMapViewport().setCenter(mercator_coords.getX(), app_state.getMapViewport().getCenterY());
            app_state.getMapViewport().applyConstraints();
            app_state.getMapViewport().notifyViewportChanged();
        });

        // y <latitude>: 緯度を設定（範囲: 0.0～90.0）
        console.registerCommand("y", [&app_state](const std::vector<std::string>& args) {
            if (args.size() < 2) {
                PAXS_WARNING("Usage: y <latitude> (range: 0.0-90.0)");
                return;
            }
            auto latitude_opt = StringUtils::toDouble(args[1]);
            if (!latitude_opt) {
                PAXS_ERROR("Invalid latitude value: \"" + args[1] + "\"");
                return;
            }
            double latitude = *latitude_opt;
            if (latitude < 0.0 || latitude > 90.0) {
                PAXS_WARNING("Latitude must be between 0.0 and 90.0");
                return;
            }
            // メルカトル座標に変換
            paxs::Vector2<double> equirect_coords(app_state.getMapViewport().getCenterX(), latitude);
            paxg::Coordinate mercator_coords = paxs::MercatorDeg(paxs::EquirectangularDeg(equirect_coords));
            app_state.getMapViewport().setCenter(app_state.getMapViewport().getCenterX(), mercator_coords.getY());
            app_state.getMapViewport().applyConstraints();
            app_state.getMapViewport().notifyViewportChanged();
        });

        // z <zoom>: 拡大率を設定
        console.registerCommand("z", [&app_state](const std::vector<std::string>& args) {
            if (args.size() < 2) {
                PAXS_WARNING("Usage: z <zoom> (range: " +
                    std::to_string(app_state.getMapViewport().getMinHeight()) + "-" +
                    std::to_string(app_state.getMapViewport().getMaxHeight()) + ")");
                return;
            }
            auto zoom_opt = StringUtils::toDouble(args[1]);
            if (!zoom_opt) {
                PAXS_ERROR("Invalid zoom value: \"" + args[1] + "\"");
                return;
            }
            double zoom = *zoom_opt;
            const double min_h = app_state.getMapViewport().getMinHeight();
            const double max_h = app_state.getMapViewport().getMaxHeight();
            if (zoom < min_h || zoom > max_h) {
                PAXS_WARNING("Zoom must be between " + std::to_string(min_h) + " and " + std::to_string(max_h));
                return;
            }
            app_state.getMapViewport().setSize(zoom);
            app_state.getMapViewport().applyConstraints();
            app_state.getMapViewport().notifyViewportChanged();
        });
    }

#ifdef PAXS_USING_SIMULATOR
    /// @brief シミュレーション関連コマンドを登録
    /// @brief Register simulation-related commands
    static void registerSimulationCommands(DebugConsole& console, AppStateManager& app_state) {
        // sim init [model_name]: シミュレーションを初期化
        console.registerCommand("sim", [&app_state](const std::vector<std::string>& args) {
            if (args.size() < 2) {
                PAXS_WARNING("Usage: sim <init [model_name]|start|stop>");
                return;
            }

            const std::string& subcommand = args[1];
            if (subcommand == "init") {
                // モデル名を取得（指定されていない場合は"sample"）
                std::string model_name = "sample";
                if (args.size() >= 3) {
                    model_name = args[2];
                }
                EventBus::getInstance().publish(SimulationInitializeCommandEvent(model_name));
                PAXS_INFO("Simulation initialized with model: " + model_name);
            } else if (subcommand == "start") {
                app_state.executeTimePlaybackControl(TimePlaybackControlEvent::Action::Forward);
                PAXS_INFO("Simulation started");
            } else if (subcommand == "stop") {
                app_state.executeTimePlaybackControl(TimePlaybackControlEvent::Action::Stop);
                PAXS_INFO("Simulation stopped");
            } else {
                PAXS_WARNING("Unknown sim subcommand: " + subcommand);
            }
        });
    }
#endif // PAXS_USING_SIMULATOR
};
#endif // PAXS_DEVELOPMENT

} // namespace paxs

#endif // !PAX_MAHOROBA_UI_DEBUG_DEBUG_CONSOLE_COMMAND_REGISTRY_HPP
