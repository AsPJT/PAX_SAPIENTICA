/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_GRAPHICA_NULL_GRAPHICS_3D_MODEL_IMPL_HPP
#define PAX_GRAPHICA_NULL_GRAPHICS_3D_MODEL_IMPL_HPP

#include <PAX_GRAPHICA/Interface/Graphics3DModelImpl.hpp>

namespace paxg {

    struct Graphics3DModelConfig;

    /// @brief Null (no-op) implementation of Graphics3DModelImpl
    /// @note Used when no graphics library is available
    class NullGraphics3DModelImpl : public Graphics3DModelImpl {
    private:
        int rotation_ = 0;

    public:
        /// @brief Constructor with configuration
        /// @param cfg Configuration for 3D model (unused)
        explicit NullGraphics3DModelImpl(const Graphics3DModelConfig&) {}

        /// @brief Update rotation angle (no-op)
        void updateRotation() override {
            ++rotation_;
            if (rotation_ >= 360) rotation_ = 0;
        }

        /// @brief Render 3D model (no-op)
        void render() const override {
            // No operation
        }

        /// @brief Get current rotation angle (degrees)
        int getRotation() const override {
            return rotation_;
        }

        /// @brief Set rotation angle (degrees)
        /// @param angle Rotation angle in degrees
        void setRotation(int angle) override {
            rotation_ = angle % 360;
        }

        /// @brief Update camera configuration (no-op)
        /// @param cam Camera configuration (unused)
        void setCameraConfig(const Graphics3DModelConfig::Camera&) override {
            // No operation
        }

        /// @brief Update background configuration (no-op)
        /// @param bg Background configuration (unused)
        void setBackgroundConfig(const Graphics3DModelConfig::Background&) override {
            // No operation
        }
    };

}

#endif // !PAX_GRAPHICA_NULL_GRAPHICS_3D_MODEL_IMPL_HPP
