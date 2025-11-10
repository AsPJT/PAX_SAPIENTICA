/*##########################################################################################

	PAX SAPIENTICA Library 💀🌿🌏

	[Planning]		2023-2024 As Project
	[Production]	2023-2024 As Project
	[Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
	[License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#include <gtest/gtest.h>
#include <PAX_GRAPHICA/System.hpp>

// launchBrowser のテスト（実際にブラウザを開くわけにはいかないので、単に呼び出せることを確認）
TEST(SystemTest, LaunchBrowserDoesNotCrash) {
	// URLが空でも呼び出せることを確認
	EXPECT_NO_THROW(paxg::System::launchBrowser(""));

	// 正しいURLでも呼び出せることを確認（実際には開かない）
	EXPECT_NO_THROW(paxg::System::launchBrowser("https://github.com/AsPJT/PAX_SAPIENTICA"));
}
