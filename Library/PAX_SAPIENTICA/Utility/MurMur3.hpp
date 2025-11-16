/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_TYPE_MURMUR3_HPP
#define PAX_SAPIENTICA_TYPE_MURMUR3_HPP

#include <cstddef>
#include <cstdint>
#include <string>

namespace paxs {

    namespace mm3 {
        constexpr std::uint_least32_t size4u = 4U;
    }
    // MurMur3 ハッシュ計算クラス
    class MurMur3 {
    private:
        static constexpr std::uint_least32_t to_uint32(
            const char* const key,
            const std::size_t count = mm3::size4u,
            const std::uint_least32_t u32 = 0) {
            return count ? to_uint32(key, count - 1, (u32 << 8) | key[count - 1]) : u32;
        }

        static constexpr std::uint_least32_t murmur3a_5(const std::uint_least32_t h) {
            return h * 5 + 0xe6546b64;
        }
        static constexpr std::uint_least32_t murmur3a_4(const std::uint_least32_t h) {
            return murmur3a_5((h << 13) | (h >> 19));
        }
        static constexpr std::uint_least32_t murmur3a_3(const std::uint_least32_t k, const std::uint_least32_t h) {
            return murmur3a_4(h ^ k);
        }
        static constexpr std::uint_least32_t murmur3a_2(const std::uint_least32_t k, const std::uint_least32_t h) {
            return murmur3a_3(k * 0x1b873593, h);
        }
        static constexpr std::uint_least32_t murmur3a_1(const std::uint_least32_t k, const std::uint_least32_t h) {
            return murmur3a_2((k << 15) | (k >> 17), h);
        }
        static constexpr std::uint_least32_t murmur3a_0(const std::uint_least32_t k, const std::uint_least32_t h) {
            return murmur3a_1(k * 0xcc9e2d51, h);
        }
        static constexpr std::uint_least32_t murmur3a(
            const char* const key,
            const std::size_t count,
            const std::uint_least32_t h /*seed*/) {
            return count ? murmur3a(key + mm3::size4u, count - 1, murmur3a_0(to_uint32(key), h)) : h;
        }

        static constexpr std::uint_least32_t murmur3b_3(const std::uint_least32_t k, const std::uint_least32_t h) {
            return h ^ k;
        }
        static constexpr std::uint_least32_t murmur3b_2(const std::uint_least32_t k, const std::uint_least32_t h) {
            return murmur3b_3(k * 0x1b873593, h);
        }
        static constexpr std::uint_least32_t murmur3b_1(const std::uint_least32_t k, const std::uint_least32_t h) {
            return murmur3b_2((k << 15) | (k >> 17), h);
        }
        static constexpr std::uint_least32_t murmur3b_0(const std::uint_least32_t k, const std::uint_least32_t h) {
            return murmur3b_1(k * 0xcc9e2d51, h);
        }
        static constexpr std::uint_least32_t murmur3b(
            const char* const key,
            const std::size_t count,
            const std::uint_least32_t h) {
            return count ? murmur3b_0(to_uint32(key, count), h) : h;
        }

        static constexpr std::uint_least32_t murmur3c_4(const std::uint_least32_t h) {
            return h ^ (h >> 16);
        }
        static constexpr std::uint_least32_t murmur3c_3(const std::uint_least32_t h) {
            return murmur3c_4(h * 0xc2b2ae35);
        }
        static constexpr std::uint_least32_t murmur3c_2(const std::uint_least32_t h) {
            return murmur3c_3(h ^ (h >> 13));
        }
        static constexpr std::uint_least32_t murmur3c_1(const std::uint_least32_t h) {
            return murmur3c_2(h * 0x85ebca6b);
        }
        static constexpr std::uint_least32_t murmur3c_0(const std::uint_least32_t h) {
            return murmur3c_1(h ^ (h >> 16));
        }
        static constexpr std::uint_least32_t murmur3c(const std::uint_least32_t h, const std::size_t len) {
            return murmur3c_0(h ^ static_cast<std::uint_least32_t>(len));
        }

        static constexpr std::uint_least32_t murmur3_Rotate(std::uint_least32_t value, std::size_t r) {
            return (value << r) | (value >> (32 - r));
        }
        static constexpr std::uint_least32_t murmur3_Chunk(const char* const str, std::uint_least32_t h) {
            return murmur3a_5(murmur3_Rotate(murmur3b(str, 4, h), 13));
        }
        static constexpr std::uint_least32_t calcHashStack(
            const char* const str, const std::uint_least32_t h, const std::size_t count) {
            return ((str[0] == '\0') ? murmur3c(h, count) :
                ((str[1] == '\0') ? murmur3c(murmur3b(str, 1, h), count + 1) :
                    ((str[2] == '\0') ? murmur3c(murmur3b(str, 2, h), count + 2) :
                        ((str[3] == '\0') ? murmur3c(murmur3b(str, 3, h), count + 3)
                            : calcHashStack(str + 4, murmur3_Chunk(str, h), count + 4)))));
        }

    public:
        // コンパイル時に文字列の MurMur3 ハッシュを計算
        static constexpr std::uint_least32_t calcHash(const std::size_t len, const char* const str, const std::uint_least32_t seed = 0) {
            return murmur3c(
                murmur3b(
                    str + ((len >> 2) * mm3::size4u), len & 3, murmur3a(str, len >> 2, seed)
                )
                , len);
        }
        static constexpr std::uint_least32_t calcHash(const std::string& str, const std::uint_least32_t seed = 0) {
            return calcHash(str.size(), str.c_str(), seed);
        }
        // 文字列の長さを計算する（スタックによる実装）
        static constexpr std::uint_least32_t calcHashStack(const char* const str, const std::uint_least32_t seed = 0) {
            return calcHashStack(str, seed, 0);
        }
        // 文字列の長さを計算する
        static constexpr std::uint_least32_t calcHash(const char* const str, const std::uint_least32_t seed = 0) {
            std::uint_least32_t h = seed; std::size_t len = 0;
            for (;; len += 4) {
                if (str[len] == '\0') {
                    break;
                }
                if (str[len + 1] == '\0') {
                    h = murmur3b(str + len, 1, h);
                    len += 1;
                    break;
                }
                if (str[len + 2] == '\0') {
                    h = murmur3b(str + len, 2, h);
                    len += 2;
                    break;
                }
                if (str[len + 3] == '\0') {
                    h = murmur3b(str + len, 3, h);
                    len += 3;
                    break;
                }
                h = murmur3_Chunk(str + len, h);
            }
            return murmur3c(h, len);
        }

    };

}

#endif // !PAX_SAPIENTICA_TYPE_MURMUR3_HPP
