/*##########################################################################################

	PAX SAPIENTICA Library 💀🌿🌏

	[Planning]		2023-2024 As Project
	[Production]	2023-2024 As Project
	[Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
	[License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_TOUCH_STATE_MANAGER_HPP
#define PAX_SAPIENTICA_TOUCH_STATE_MANAGER_HPP

/*##########################################################################################
	このファイルは後方互換性のために残されています。
	新しいコードでは InputStateManager を使用してください。

	This file is kept for backward compatibility.
	Please use InputStateManager for new code.
##########################################################################################*/

#include <PAX_SAPIENTICA/InputStateManager.hpp>

namespace paxs {

	/// @deprecated TouchStateManager is deprecated. Use InputStateManager instead.
	/// @brief TouchStateManager は非推奨です。代わりに InputStateManager を使用してください。
	using TouchStateManager = InputStateManager;

}

#endif // !PAX_SAPIENTICA_TOUCH_STATE_MANAGER_HPP
