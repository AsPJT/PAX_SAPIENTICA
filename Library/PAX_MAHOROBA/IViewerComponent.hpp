/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_MAHOROBA_I_VIEWER_COMPONENT_HPP
#define PAX_MAHOROBA_I_VIEWER_COMPONENT_HPP

namespace paxs {

    /// @brief Viewer系コンポーネントの共通インターフェース
    class IViewerComponent {
    public:
        virtual ~IViewerComponent() = default;

        /// @brief コンポーネントの名前を取得
        virtual const char* getName() const = 0;

        /// @brief このコンポーネントが利用可能かどうか
        /// @return 利用可能な場合true（例: 条件付きコンパイルで有効な場合）
        virtual bool isAvailable() const = 0;

        /// @brief コンポーネントが有効化されているかどうか
        /// @return 有効化されている場合true
        virtual bool isEnabled() const = 0;

        /// @brief コンポーネントを有効/無効化
        /// @param enabled 有効化する場合true
        virtual void setEnabled(bool enabled) = 0;
    };

} // namespace paxs

#endif // !PAX_MAHOROBA_I_VIEWER_COMPONENT_HPP
