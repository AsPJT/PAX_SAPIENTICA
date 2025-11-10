/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#include <gtest/gtest.h>

#include <PAX_GRAPHICA/Image.hpp>
#include <PAX_GRAPHICA/String.hpp>

// Test Image construction with invalid path
TEST(ImageUnitTest, InvalidPathConstruction) {
    paxg::String invalidPath("nonexistent_file.png");
    paxg::Image img(invalidPath);

#if defined(PAXS_USING_SIV3D) || defined(PAXS_USING_SFML)
    // Image should be invalid for libraries that support validation
    ASSERT_FALSE(img.isValid());
    ASSERT_FALSE(static_cast<bool>(img));
#else
    // Null implementation always returns false
    ASSERT_FALSE(img.isValid());
#endif
}

// Test Image validity check
TEST(ImageUnitTest, ValidityCheck) {
    paxg::String invalidPath("nonexistent.png");
    paxg::Image img(invalidPath);

    ASSERT_FALSE(img.isValid());
    ASSERT_FALSE(static_cast<bool>(img));
}

// Test Image bool conversion operator
TEST(ImageUnitTest, BoolConversion) {
    paxg::String invalidPath("invalid.png");
    paxg::Image img(invalidPath);

    // explicit operator bool
    if (img) {
        FAIL() << "Invalid image should evaluate to false";
    } else {
        SUCCEED();
    }
}

#if defined(PAXS_USING_SFML)
// Test SFML 3.0.0 specific features
TEST(ImageUnitTest, SFML_SaveToMemory) {
    // Test with invalid image
    paxg::String invalidPath("nonexistent.png");
    paxg::Image img(invalidPath);

    // Should return std::nullopt or empty optional
    auto result = img.saveToMemory("png");
    ASSERT_FALSE(result.has_value() && !result->empty());
}

TEST(ImageUnitTest, SFML_SaveToFile) {
    paxg::String invalidPath("nonexistent.png");
    paxg::Image img(invalidPath);

    // Should fail to save invalid image
    bool saveResult = img.saveToFile("/tmp/test_output.png");
    ASSERT_FALSE(saveResult);
}

TEST(ImageUnitTest, SFML_Validation) {
    paxg::String invalidPath("nonexistent.png");
    paxg::Image img(invalidPath);

    // SFML validates by checking size > 0
    ASSERT_FALSE(img.isValid());
}
#endif

// Test default behavior for undefined graphics library
#if !defined(PAXS_USING_SIV3D) && !defined(PAXS_USING_DXLIB) && !defined(PAXS_USING_SFML)
TEST(ImageUnitTest, NullImplementation) {
    paxg::String path("any_path.png");
    paxg::Image img(path);

    // Null implementation always returns false
    ASSERT_FALSE(img.isValid());
}
#endif
