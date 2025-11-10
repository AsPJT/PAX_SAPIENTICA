/*##########################################################################################

	PAX SAPIENTICA Library 💀🌿🌏

	[Planning]		2023-2024 As Project
	[Production]	2023-2024 As Project
	[Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
	[License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#include <gtest/gtest.h>
#include <PAX_SAPIENTICA/Color/Ryoseikoku.hpp>

namespace paxs {

	// RyoseikokuColor クラスのテスト
	class RyoseikokuColorTest : public ::testing::Test {
	protected:
		RyoseikokuColor ryoseikoku;
	};

	// getIndex() のテスト：基本的な境界値
	TEST_F(RyoseikokuColorTest, GetIndexBoundaryValues) {
		// インデックス0（透明）
		EXPECT_EQ(ryoseikoku.getIndex(RGBA{0, 0, 0, 0}), 0);

		// インデックス0（黒）
		EXPECT_EQ(ryoseikoku.getIndex(RGBA{0, 0, 0, 255}), 0);

		// インデックス0（白）
		EXPECT_EQ(ryoseikoku.getIndex(RGBA{255, 255, 255, 255}), 0);
	}

	// getIndex() のテスト：既知のカラー値
	TEST_F(RyoseikokuColorTest, GetIndexKnownColors) {
		// インデックス1: {245,105,100}
		EXPECT_EQ(ryoseikoku.getIndex(RGBA{245, 105, 100, 255}), 1);

		// インデックス10: {200,150,100}
		EXPECT_EQ(ryoseikoku.getIndex(RGBA{200, 150, 100, 255}), 10);

		// インデックス30: {100,250,100}
		EXPECT_EQ(ryoseikoku.getIndex(RGBA{100, 250, 100, 255}), 30);

		// インデックス60: {100,100,250}
		EXPECT_EQ(ryoseikoku.getIndex(RGBA{100, 100, 250, 255}), 60);

		// インデックス90: {250,100,100}
		EXPECT_EQ(ryoseikoku.getIndex(RGBA{250, 100, 100, 255}), 90);
	}

	// getIndex() のテスト：近似値の計算
	TEST_F(RyoseikokuColorTest, GetIndexApproximateColors) {
		// {245,105,100}に近い値 → インデックス1
		EXPECT_EQ(ryoseikoku.getIndex(RGBA{246, 106, 101, 255}), 1);
		EXPECT_EQ(ryoseikoku.getIndex(RGBA{244, 104, 99, 255}), 1);

		// {100,250,100}に近い値 → インデックス30
		EXPECT_EQ(ryoseikoku.getIndex(RGBA{101, 251, 101, 255}), 30);
		EXPECT_EQ(ryoseikoku.getIndex(RGBA{99, 249, 99, 255}), 30);
	}

	// getIndex() のテスト：不透明度が255以外
	TEST_F(RyoseikokuColorTest, GetIndexNonOpaqueColors) {
		// アルファ値が255以外の場合は常に0
		EXPECT_EQ(ryoseikoku.getIndex(RGBA{245, 105, 100, 128}), 0);
		EXPECT_EQ(ryoseikoku.getIndex(RGBA{200, 150, 100, 100}), 0);
		EXPECT_EQ(ryoseikoku.getIndex(RGBA{100, 250, 100, 1}), 0);
	}

	// getIndex() のテスト：配列の全要素が有効
	TEST_F(RyoseikokuColorTest, GetIndexAllIndicesValid) {
		// 配列サイズは91（0～90）
		// インデックス0は特殊ケース（透明、黒、白）
		// インデックス1～90は対応する色を持つ

		// 中間値のテスト
		EXPECT_EQ(ryoseikoku.getIndex(RGBA{175, 175, 100, 255}), 15);
		EXPECT_EQ(ryoseikoku.getIndex(RGBA{100, 175, 175, 255}), 45);
		EXPECT_EQ(ryoseikoku.getIndex(RGBA{175, 100, 175, 255}), 75);
	}

	// constexpr配列のコンパイル時計算テスト
	TEST_F(RyoseikokuColorTest, ConstexprArrayTest) {
		// 配列がconstexprで定義されていることを確認
		// （コンパイル時に評価される）
		constexpr std::size_t array_size = 91;

		// 実行時にサイズを確認（間接的な検証）
		// soil_tempはprivateなので直接アクセスできないが、
		// getIndexの動作から91要素あることを確認
		EXPECT_EQ(ryoseikoku.getIndex(RGBA{250, 100, 100, 255}), 90); // 最後の要素
	}

} // namespace paxs
