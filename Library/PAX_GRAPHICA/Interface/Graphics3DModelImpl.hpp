/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_GRAPHICA_GRAPHICS_3D_MODEL_IMPL_HPP
#define PAX_GRAPHICA_GRAPHICS_3D_MODEL_IMPL_HPP

#include <string>

namespace paxg {

    /// @brief 3D モデル描画の設定
    struct Graphics3DModelConfig {
        /// @brief カメラ設定
        struct Camera {
            /// @brief 垂直視野角（度）
            double verticalFOV = 40.0;
            double posX = 0.0;
            double posY = 3.0;
            double posZ = -16.0;
        } camera;

        /// @brief 背景色設定
        struct Background {
            float r = 1.0f;                 // 赤成分
            float g = 1.0f;                 // 緑成分
            float b = 1.0f;                 // 青成分
            float a = 0.0f;                 // 透明度
        } background;

        /// @brief ファイルパス設定
        struct FilePaths {
            std::string modelPath = "Data/3DModels/KofunOBJ/Model/Sekishitsu/KamoKitaKofun/KamoKitaKofun.obj";
        } paths;

        /// @brief モデルの回転速度（度/フレーム）
        double rotationSpeed = 1.0;
    };

    /// @brief Abstract base class for Graphics3DModel implementation (Strategy pattern)
    /// @note Each graphics library provides its own implementation
    class Graphics3DModelImpl {
    public:
        virtual ~Graphics3DModelImpl() = default;

        /// @brief Update rotation angle
        virtual void updateRotation() = 0;

        /// @brief Render 3D model
        virtual void render() const = 0;

        /// @brief Get current rotation angle (degrees)
        virtual int getRotation() const = 0;

        /// @brief Set rotation angle (degrees)
        /// @param angle Rotation angle in degrees
        virtual void setRotation(int angle) = 0;

        /// @brief Update camera configuration
        /// @param cam Camera configuration
        virtual void setCameraConfig(const Graphics3DModelConfig::Camera& cam) = 0;

        /// @brief Update background configuration
        /// @param bg Background configuration
        virtual void setBackgroundConfig(const Graphics3DModelConfig::Background& bg) = 0;
    };

}

#endif // !PAX_GRAPHICA_GRAPHICS_3D_MODEL_IMPL_HPP
