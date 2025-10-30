/*##########################################################################################

	PAX SAPIENTICA Library 💀🌿🌏

	[Planning]		2023-2024 As Project
	[Production]	2023-2024 As Project
	[Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
	[License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#include <gtest/gtest.h>
#include <PAX_GRAPHICA/Network.hpp>
#include <filesystem>

// downloadFile のテスト（実際にダウンロードするわけにはいかないので、単に呼び出せることを確認）
TEST(NetworkTest, DownloadFileDoesNotCrash) {
	// 空のURLでも呼び出せることを確認
	EXPECT_NO_THROW(paxg::Network::downloadFile("", ""));

	// 不正なURLでも呼び出せることを確認（失敗するだけでクラッシュしない）
	EXPECT_NO_THROW(paxg::Network::downloadFile("invalid_url", "test_output.tmp"));

	// テストファイルが存在する場合は削除
	if (std::filesystem::exists("test_output.tmp")) {
		std::filesystem::remove("test_output.tmp");
	}
}
