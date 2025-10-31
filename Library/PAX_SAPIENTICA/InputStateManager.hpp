/*##########################################################################################

	PAX SAPIENTICA Library 💀🌿🌏

	[Planning]		2023-2024 As Project
	[Production]	2023-2024 As Project
	[Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
	[License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_INPUT_STATE_MANAGER_HPP
#define PAX_SAPIENTICA_INPUT_STATE_MANAGER_HPP

namespace paxs {

	/// @brief 入力状態を管理して二重処理を防ぐクラス (Infrastructure Layer)
	/// @brief A class that manages input state (mouse/touch) to prevent duplicate processing
	/// @details マウスやタッチなどの入力イベントが処理済みかどうかを追跡し、同じ入力を複数回処理することを防ぎます
	/// @details Tracks whether an input event has been processed to avoid handling the same input multiple times
	struct InputStateManager {
	private:
		// 入力されているか (マウスクリックやタッチなど)
		bool is_input = false;
	public:
		/// @brief 入力状態を初期化する
		/// @brief Initialize the input state
		/// @details 内部の入力フラグをリセットして、新しい入力イベントを受け付けられるようにします
		/// @details Resets the internal input flag to allow new input events
		constexpr void init() noexcept { is_input = false; }

		/// @brief 入力状態を確認して更新する
		/// @brief Check and update input state
		/// @param is_input_ 現在の入力状態 (true = 入力あり, false = 入力なし)
		/// @param is_input_ Current input status (true if input detected, false otherwise)
		/// @return true = 新しい入力イベント（未処理）, false = 既に処理済みまたは入力なし
		/// @return true if this is a new input event (not yet processed), false if already processed or no input
		/// @details 入力が開始された最初のフレームでのみtrueを返し、二重処理を防ぎます
		/// @details Returns true only on the first frame when input begins, preventing duplicate processing
		constexpr bool get(const bool is_input_) noexcept {

			// まだ入力されていない場合
			if (!is_input) {
				// 今回も入力されていないので不可
				if (!is_input_) {
					return false;
				}
				// 今回初めて入力されたので成功
				else {
					is_input = true;
					return true;
				}
			}
			// 既に入力されている場合
			else {
				// 二重処理を防ぐため、不可
				return false;
			}
		}
	};
}

#endif // !PAX_SAPIENTICA_INPUT_STATE_MANAGER_HPP
