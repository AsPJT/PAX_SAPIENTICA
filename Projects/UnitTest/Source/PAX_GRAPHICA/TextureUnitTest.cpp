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
#include <PAX_GRAPHICA/Texture.hpp>

// Test Texture default construction
TEST(TextureUnitTest, DefaultConstruction) {
    paxg::Texture texture;

    // Default constructed texture should be invalid
#if defined(PAXS_USING_SIV3D) || defined(PAXS_USING_DXLIB) || defined(PAXS_USING_SFML)
    ASSERT_FALSE(static_cast<bool>(texture));
    ASSERT_TRUE(!texture);
#else
    // Null implementation
    ASSERT_FALSE(static_cast<bool>(texture));
#endif
}

// Test Texture construction with invalid path
TEST(TextureUnitTest, InvalidPathConstruction) {
    paxg::String invalidPath("nonexistent_texture.png");
    paxg::Texture texture(invalidPath);

#if defined(PAXS_USING_SIV3D) || defined(PAXS_USING_SFML) || defined(PAXS_USING_DXLIB)
    // Texture from invalid path should be invalid
    ASSERT_FALSE(static_cast<bool>(texture));
    ASSERT_TRUE(!texture);
#else
    ASSERT_FALSE(static_cast<bool>(texture));
#endif
}

// Test Texture construction with std::string
TEST(TextureUnitTest, StdStringConstruction) {
    std::string invalidPath = "nonexistent.png";
    paxg::Texture texture(invalidPath);

    ASSERT_FALSE(static_cast<bool>(texture));
}

// Test Texture construction from Image
TEST(TextureUnitTest, ImageConstruction) {
    paxg::String invalidPath("invalid.png");
    paxg::Image img(invalidPath);
    paxg::Texture texture(img);

    // Texture from invalid image should be invalid
    ASSERT_FALSE(static_cast<bool>(texture));
}

// Test Texture width and height for invalid texture
TEST(TextureUnitTest, WidthHeightInvalid) {
    paxg::Texture texture;

    // Invalid texture should have 0 dimensions
    ASSERT_EQ(texture.width(), 0);
    ASSERT_EQ(texture.height(), 0);
}

// Test Texture bool conversion operator
TEST(TextureUnitTest, BoolConversion) {
    paxg::Texture texture;

    if (texture) {
        FAIL() << "Default constructed texture should be invalid";
    }

    if (!texture) {
        SUCCEED();
    } else {
        FAIL() << "Negated invalid texture should be true";
    }
}

// Test Texture pimpl idiom (implementation pointer)
TEST(TextureUnitTest, PimplIdiom) {
    // Create multiple textures to ensure each has its own implementation
    paxg::Texture t1;
    paxg::Texture t2;
    paxg::Texture t3;

    // All should be independently invalid
    ASSERT_FALSE(static_cast<bool>(t1));
    ASSERT_FALSE(static_cast<bool>(t2));
    ASSERT_FALSE(static_cast<bool>(t3));
}

// Test Texture copy construction (shared_ptr should handle this)
TEST(TextureUnitTest, CopyConstruction) {
    paxg::String path("test.png");
    paxg::Texture t1(path);
    paxg::Texture t2 = t1;

    // Both should have same validity
    ASSERT_EQ(static_cast<bool>(t1), static_cast<bool>(t2));
}

// Test Texture assignment (shared_ptr should handle this)
TEST(TextureUnitTest, Assignment) {
    paxg::String path1("test1.png");
    paxg::String path2("test2.png");

    paxg::Texture t1(path1);
    paxg::Texture t2(path2);

    t2 = t1;

    // After assignment, both should have same validity
    ASSERT_EQ(static_cast<bool>(t1), static_cast<bool>(t2));
}

// Test draw methods don't crash with invalid texture
TEST(TextureUnitTest, DrawMethodsSafe) {
    paxg::Texture texture;

    // These should not crash even with invalid texture
    ASSERT_NO_THROW(texture.draw());
    ASSERT_NO_THROW(texture.drawAt(paxs::Vector2<float>(0.0f, 0.0f)));
    ASSERT_NO_THROW(texture.drawAt(paxs::Vector2<int>(0, 0)));
}

// Test resized draw methods don't crash with invalid texture
TEST(TextureUnitTest, ResizedDrawMethodsSafe) {
    paxg::Texture texture;

    // These should not crash even with invalid texture
    ASSERT_NO_THROW(texture.resizedDrawAt(paxs::Vector2<int>(100, 100), paxs::Vector2<int>(0, 0)));
    ASSERT_NO_THROW(texture.resizedDrawAt(2, paxs::Vector2<int>(0, 0)));
    ASSERT_NO_THROW(texture.resizedDrawAt(paxs::Vector2<float>(2.0f, 2.0f), paxs::Vector2<float>(0.0f, 0.0f)));
    ASSERT_NO_THROW(texture.resizedDrawAt(2, paxs::Vector2<float>(0.0f, 0.0f)));

    ASSERT_NO_THROW(texture.resizedDraw(paxs::Vector2<int>(100, 100), paxs::Vector2<int>(0, 0)));
    ASSERT_NO_THROW(texture.resizedDraw(2, paxs::Vector2<int>(0, 0)));
    ASSERT_NO_THROW(texture.resizedDraw(paxs::Vector2<float>(2.0f, 2.0f), paxs::Vector2<float>(0.0f, 0.0f)));
    ASSERT_NO_THROW(texture.resizedDraw(2, paxs::Vector2<float>(0.0f, 0.0f)));
}

#if defined(PAXS_USING_SIV3D)
// Test Siv3D native type conversion
TEST(TextureUnitTest, Siv3DNativeConversion) {
    paxg::Texture texture;

    // Should be convertible to s3d::Texture
    s3d::Texture nativeTexture = static_cast<s3d::Texture>(texture);

    // Converted texture should also be invalid
    ASSERT_FALSE(static_cast<bool>(nativeTexture));
}
#endif

#if defined(PAXS_USING_SFML)
// Test SFML native type conversion
TEST(TextureUnitTest, SFMLNativeConversion) {
    paxg::Texture texture;

    // Should be convertible to sf::Texture
    sf::Texture nativeTexture = static_cast<sf::Texture>(texture);

    // Just verify conversion works without crashing
    SUCCEED();
}
#endif
