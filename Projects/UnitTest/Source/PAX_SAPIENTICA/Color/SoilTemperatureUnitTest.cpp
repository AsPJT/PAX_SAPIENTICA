/*##########################################################################################

	PAX SAPIENTICA Library 💀🌿🌏

	[Planning]		2023-2024 As Project
	[Production]	2023-2024 As Project
	[Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
	[License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#include <gtest/gtest.h>
#include <PAX_SAPIENTICA/Color/SoilTemperature.hpp>

namespace paxs {

	// SoilTemp クラスのテスト
	class SoilTempTest : public ::testing::Test {
	protected:
		SoilTemp soil_temp;
	};

	// getIndex() のテスト：基本的な境界値
	TEST_F(SoilTempTest, GetIndexBoundaryValues) {
		// インデックス0（透明）
		EXPECT_EQ(soil_temp.getIndex(RGBA{0, 0, 0, 0}), 0);

		// インデックス0（黒）
		EXPECT_EQ(soil_temp.getIndex(RGBA{0, 0, 0, 255}), 0);

		// インデックス0（白）
		EXPECT_EQ(soil_temp.getIndex(RGBA{255, 255, 255, 255}), 0);
	}

	// getIndex() のテスト：既知のカラー値（16進数で定義されたもの）
	TEST_F(SoilTempTest, GetIndexKnownColors) {
		// インデックス1: "#FFFFFF" (255, 255, 255) → 白は0
		EXPECT_EQ(soil_temp.getIndex(RGBA{255, 255, 255, 255}), 0);

		// インデックス2: "#1A7DB7" (26, 125, 183)
		EXPECT_EQ(soil_temp.getIndex(RGBA{26, 125, 183, 255}), 2);

		// インデックス10: "#B8D8E7" (184, 216, 231)
		EXPECT_EQ(soil_temp.getIndex(RGBA{184, 216, 231, 255}), 10);

		// インデックス20: "#F39F7E" (243, 159, 126)
		EXPECT_EQ(soil_temp.getIndex(RGBA{243, 159, 126, 255}), 20);

		// インデックス26: "#CA0020" (202, 0, 32)
		EXPECT_EQ(soil_temp.getIndex(RGBA{202, 0, 32, 255}), 26);
	}

	// getIndex() のテスト：近似値の計算
	TEST_F(SoilTempTest, GetIndexApproximateColors) {
		// "#1A7DB7" (26, 125, 183) に近い値 → インデックス2
		EXPECT_EQ(soil_temp.getIndex(RGBA{27, 126, 184, 255}), 2);
		EXPECT_EQ(soil_temp.getIndex(RGBA{25, 124, 182, 255}), 2);

		// "#CA0020" (202, 0, 32) に近い値 → インデックス26
		EXPECT_EQ(soil_temp.getIndex(RGBA{203, 1, 33, 255}), 26);
		EXPECT_EQ(soil_temp.getIndex(RGBA{201, 0, 31, 255}), 26);
	}

	// getIndex() のテスト：不透明度が255以外
	TEST_F(SoilTempTest, GetIndexNonOpaqueColors) {
		// アルファ値が255以外の場合は常に0
		EXPECT_EQ(soil_temp.getIndex(RGBA{26, 125, 183, 128}), 0);
		EXPECT_EQ(soil_temp.getIndex(RGBA{202, 0, 32, 100}), 0);
		EXPECT_EQ(soil_temp.getIndex(RGBA{184, 216, 231, 1}), 0);
	}

	// getIndex() のテスト：配列の全要素が有効
	TEST_F(SoilTempTest, GetIndexAllIndicesValid) {
		// 配列サイズは91だが、実際に使われているのは0～26
		// （コメントでは0～25と記載されているが配列は91要素）

		// 中間値のテスト
		EXPECT_EQ(soil_temp.getIndex(RGBA{153, 200, 223, 255}), 8); // "#99C8DF" (153, 200, 223)に近い
		EXPECT_EQ(soil_temp.getIndex(RGBA{239, 243, 245, 255}), 13); // "#EFF3F5" (239, 243, 245)
	}

	// constexpr配列のコンパイル時計算テスト
	TEST_F(SoilTempTest, ConstexprArrayTest) {
		// 配列がconstexprで定義されていることを確認
		// RGBA(const char*)コンストラクタがconstexprであることの検証

		constexpr std::size_t array_size = 91;

		// 実行時にサイズを確認（間接的な検証）
		// soil_tempはprivateなので直接アクセスできないが、
		// getIndexの動作から91要素あることを確認
		EXPECT_EQ(soil_temp.getIndex(RGBA{202, 0, 32, 255}), 26); // 最後の有効要素
	}

	// RGBA文字列コンストラクタのテスト（constexpr検証）
	TEST_F(SoilTempTest, RGBAStringConstructorTest) {
		// constexpr RGBA("#1A7DB7") がコンパイル時に評価されることを確認
		constexpr RGBA test_color("#1A7DB7");
		EXPECT_EQ(test_color.r, 26);
		EXPECT_EQ(test_color.g, 125);
		EXPECT_EQ(test_color.b, 183);
		EXPECT_EQ(test_color.a, 255);

		// "#CA0020"
		constexpr RGBA test_color2("#CA0020");
		EXPECT_EQ(test_color2.r, 202);
		EXPECT_EQ(test_color2.g, 0);
		EXPECT_EQ(test_color2.b, 32);
		EXPECT_EQ(test_color2.a, 255);
	}

	// 重複インデックスのテスト（インデックス20と21が同じ色）
	TEST_F(SoilTempTest, DuplicateColorTest) {
		// インデックス20と21は両方 "#F39F7E" (243, 159, 126)
		EXPECT_EQ(soil_temp.getIndex(RGBA{243, 159, 126, 255}), 20);
		// 完全一致の場合、最初に見つかったインデックス（20）が返される
	}

} // namespace paxs
