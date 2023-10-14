/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023 As Project
    [Production]	2023 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_TYPE_MURMUR3_HPP
#define PAX_SAPIENTICA_TYPE_MURMUR3_HPP

/*##########################################################################################

##########################################################################################*/

#include <cstddef>
#include <cstdint>

#include <string>

namespace paxs {
    // MurMur3 ハッシュ計算関数
    namespace mm3 {

        constexpr std::uint_least32_t seed = 0;
        constexpr std::uint_least32_t size4u = 4U;

        constexpr std::uint_least32_t to_uint32(
            const char* const key,
            const std::size_t i = size4u,
            const std::uint_least32_t u32 = 0) {
            return i ? to_uint32(key, i - 1, (u32 << 8) | key[i - 1]) : u32;
        }

        constexpr std::uint_least32_t murmur3a_5(const std::uint_least32_t h) {
            return h * 5 + 0xe6546b64;
        }
        constexpr std::uint_least32_t murmur3a_4(const std::uint_least32_t h) {
            return murmur3a_5((h << 13) | (h >> 19));
        }
        constexpr std::uint_least32_t murmur3a_3(const std::uint_least32_t k, const std::uint_least32_t h) {
            return murmur3a_4(h ^ k);
        }
        constexpr std::uint_least32_t murmur3a_2(const std::uint_least32_t k, const std::uint_least32_t h) {
            return murmur3a_3(k * 0x1b873593, h);
        }
        constexpr std::uint_least32_t murmur3a_1(const std::uint_least32_t k, const std::uint_least32_t h) {
            return murmur3a_2((k << 15) | (k >> 17), h);
        }
        constexpr std::uint_least32_t murmur3a_0(const std::uint_least32_t k, const std::uint_least32_t h) {
            return murmur3a_1(k * 0xcc9e2d51, h);
        }
        constexpr std::uint_least32_t murmur3a(
            const char* const key,
            const std::size_t i,
            const std::uint_least32_t h = seed) {
            return i ? murmur3a(key + size4u, i - 1, murmur3a_0(to_uint32(key), h)) : h;
        }

        constexpr std::uint_least32_t murmur3b_3(const std::uint_least32_t k, const std::uint_least32_t h) {
            return h ^ k;
        }
        constexpr std::uint_least32_t murmur3b_2(const std::uint_least32_t k, const std::uint_least32_t h) {
            return murmur3b_3(k * 0x1b873593, h);
        }
        constexpr std::uint_least32_t murmur3b_1(const std::uint_least32_t k, const std::uint_least32_t h) {
            return murmur3b_2((k << 15) | (k >> 17), h);
        }
        constexpr std::uint_least32_t murmur3b_0(const std::uint_least32_t k, const std::uint_least32_t h) {
            return murmur3b_1(k * 0xcc9e2d51, h);
        }
        constexpr std::uint_least32_t murmur3b(
            const char* const key,
            const std::size_t i,
            const std::uint_least32_t h) {
            return i ? murmur3b_0(to_uint32(key, i), h) : h;
        }

        constexpr std::uint_least32_t murmur3c_4(const std::uint_least32_t h) {
            return h ^ (h >> 16);
        }
        constexpr std::uint_least32_t murmur3c_3(const std::uint_least32_t h) {
            return murmur3c_4(h * 0xc2b2ae35);
        }
        constexpr std::uint_least32_t murmur3c_2(const std::uint_least32_t h) {
            return murmur3c_3(h ^ (h >> 13));
        }
        constexpr std::uint_least32_t murmur3c_1(const std::uint_least32_t h) {
            return murmur3c_2(h * 0x85ebca6b);
        }
        constexpr std::uint_least32_t murmur3c_0(const std::uint_least32_t h) {
            return murmur3c_1(h ^ (h >> 16));
        }
        constexpr std::uint_least32_t murmur3c(const std::uint_least32_t h, const std::size_t len) {
            return murmur3c_0(h ^ static_cast<std::uint_least32_t>(len));
        }

    }
    // コンパイル時に文字列の MurMur3 ハッシュを計算
    constexpr std::uint_least32_t murmur3(const char* const str, const std::size_t len) {
        return mm3::murmur3c(
            mm3::murmur3b(
                str + ((len >> 2) * mm3::size4u), len & 3, mm3::murmur3a(str, len >> 2)
            )
            , len);
    }
    // 実行時に文字列の MurMur3 ハッシュを計算
    std::uint_least32_t murmur3(const std::string& str) {
        return murmur3(str.c_str(), str.size());
    }

}

#endif // !PAX_SAPIENTICA_TYPE_MURMUR3_HPP
