/*##########################################################################################

	PAX SAPIENTICA Library 💀🌿🌏

	[Planning]		2023 As Project
	[Production]	2023 As Project
	[Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
	[License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_TOUCH_MANAGER_HPP
#define PAX_SAPIENTICA_TOUCH_MANAGER_HPP

/*##########################################################################################

##########################################################################################*/

namespace paxs {
	
	/// @brief A class that manages touch.
	/// @brief タッチを管理するクラス
	struct TouchManager {
	private:
		// タッチされているか
		bool is_touch = false;
	public:
		/// @brief Initialize the touch.
		/// @brief タッチを初期化する
		constexpr void init() noexcept { is_touch = false; }

		/// @brief Get the touch.
		/// @brief タッチを取得する
		constexpr bool get(const bool is_touch_) noexcept {

			// まだタッチされていない場合
			if (!is_touch) {
				// 今回もタッチされていないので不可
				if (!is_touch_) {
					return false;
				}
				// 今回初めてタッチされたので成功
				else {
					is_touch = true;
					return true;
				}
			}
			// 既にタッチされている場合
			else {
				// 二重処理を防ぐため、不可
				return false;
			}
		}
	};

}

#endif // !PAX_SAPIENTICA_TOUCH_MANAGER_HPP