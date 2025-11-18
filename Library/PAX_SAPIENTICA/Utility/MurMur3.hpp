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

    namespace MM3 {
        // MurMur3 algorithm constants
        constexpr std::uint_least32_t chunk_size = 4;
        constexpr std::uint_least32_t const_1 = 0xcc9e2d51;
        constexpr std::uint_least32_t const_2 = 0x1b873593;
        constexpr std::uint_least32_t multiplier = 0xe6546b64;
        constexpr std::uint_least32_t final_mix_1 = 0x85ebca6b;
        constexpr std::uint_least32_t final_mix_2 = 0xc2b2ae35;
        constexpr std::uint_least32_t rotate_13 = 13;
        constexpr std::uint_least32_t rotate_15 = 15;
        constexpr std::uint_least32_t rotate_16 = 16;
        constexpr std::uint_least32_t rotate_17 = 17;
        constexpr std::uint_least32_t rotate_19 = 19;
        constexpr std::uint_least32_t multiply_5 = 5;
        constexpr std::uint_least32_t shift_8 = 8;
        constexpr std::uint_least32_t shift_13 = 13;
        constexpr std::uint_least32_t shift_16 = 16;
        constexpr std::uint_least32_t bits_32 = 32;
    }
    // MurMur3 ハッシュ計算クラス
    class MurMur3 {
    private:
        static constexpr std::uint_least32_t to_uint32(
            const char* const key,
            const std::size_t count = MM3::chunk_size,
            const std::uint_least32_t result = 0) {
            return count != 0 ? to_uint32(key, count - 1, (result << MM3::shift_8) | key[count - 1]) : result;
        }

        static constexpr std::uint_least32_t murmur3a_5(const std::uint_least32_t hash) {
            return (hash * MM3::multiply_5) + MM3::multiplier;
        }
        static constexpr std::uint_least32_t murmur3a_4(const std::uint_least32_t hash) {
            return murmur3a_5((hash << MM3::rotate_13) | (hash >> MM3::rotate_19));
        }
        static constexpr std::uint_least32_t murmur3a_3(const std::uint_least32_t key_value, const std::uint_least32_t hash) {
            return murmur3a_4(hash ^ key_value);
        }
        static constexpr std::uint_least32_t murmur3a_2(const std::uint_least32_t key_value, const std::uint_least32_t hash) {
            return murmur3a_3(key_value * MM3::const_2, hash);
        }
        static constexpr std::uint_least32_t murmur3a_1(const std::uint_least32_t key_value, const std::uint_least32_t hash) {
            return murmur3a_2((key_value << MM3::rotate_15) | (key_value >> MM3::rotate_17), hash);
        }
        static constexpr std::uint_least32_t murmur3a_0(const std::uint_least32_t key_value, const std::uint_least32_t hash) {
            return murmur3a_1(key_value * MM3::const_1, hash);
        }
        static constexpr std::uint_least32_t murmur3a(
            const char* const key,
            const std::size_t count,
            const std::uint_least32_t hash /*seed*/) {
            return count != 0 ? murmur3a(key + MM3::chunk_size, count - 1, murmur3a_0(to_uint32(key), hash)) : hash;
        }

        static constexpr std::uint_least32_t murmur3b_3(const std::uint_least32_t key_value, const std::uint_least32_t hash) {
            return hash ^ key_value;
        }
        static constexpr std::uint_least32_t murmur3b_2(const std::uint_least32_t key_value, const std::uint_least32_t hash) {
            return murmur3b_3(key_value * MM3::const_2, hash);
        }
        static constexpr std::uint_least32_t murmur3b_1(const std::uint_least32_t key_value, const std::uint_least32_t hash) {
            return murmur3b_2((key_value << MM3::rotate_15) | (key_value >> MM3::rotate_17), hash);
        }
        static constexpr std::uint_least32_t murmur3b_0(const std::uint_least32_t key_value, const std::uint_least32_t hash) {
            return murmur3b_1(key_value * MM3::const_1, hash);
        }
        static constexpr std::uint_least32_t murmur3b(
            const char* const key,
            const std::size_t count,
            const std::uint_least32_t hash) {
            return count != 0 ? murmur3b_0(to_uint32(key, count), hash) : hash;
        }

        static constexpr std::uint_least32_t murmur3c_4(const std::uint_least32_t hash) {
            return hash ^ (hash >> MM3::shift_16);
        }
        static constexpr std::uint_least32_t murmur3c_3(const std::uint_least32_t hash) {
            return murmur3c_4(hash * MM3::final_mix_2);
        }
        static constexpr std::uint_least32_t murmur3c_2(const std::uint_least32_t hash) {
            return murmur3c_3(hash ^ (hash >> MM3::shift_13));
        }
        static constexpr std::uint_least32_t murmur3c_1(const std::uint_least32_t hash) {
            return murmur3c_2(hash * MM3::final_mix_1);
        }
        static constexpr std::uint_least32_t murmur3c_0(const std::uint_least32_t hash) {
            return murmur3c_1(hash ^ (hash >> MM3::shift_16));
        }
        static constexpr std::uint_least32_t murmur3c(const std::uint_least32_t hash, const std::size_t length) {
            return murmur3c_0(hash ^ static_cast<std::uint_least32_t>(length));
        }

        static constexpr std::uint_least32_t murmur3_Rotate(std::uint_least32_t value, std::size_t rotation) {
            return (value << rotation) | (value >> (MM3::bits_32 - rotation));
        }
        static constexpr std::uint_least32_t murmur3_Chunk(const char* const str, std::uint_least32_t hash) {
            return murmur3a_5(murmur3_Rotate(murmur3b(str, MM3::chunk_size, hash), MM3::rotate_13));
        }
        static constexpr std::uint_least32_t calcHashStack(
            const char* const str, const std::uint_least32_t hash, const std::size_t count) {
            return ((str[0] == '\0') ? murmur3c(hash, count) :
                ((str[1] == '\0') ? murmur3c(murmur3b(str, 1, hash), count + 1) :
                    ((str[2] == '\0') ? murmur3c(murmur3b(str, 2, hash), count + 2) :
                        ((str[3] == '\0') ? murmur3c(murmur3b(str, 3, hash), count + 3)
                            : calcHashStack(str + MM3::chunk_size, murmur3_Chunk(str, hash), count + MM3::chunk_size)))));
        }

    public:
        // コンパイル時に文字列の MurMur3 ハッシュを計算
        static constexpr std::uint_least32_t calcHash(const std::size_t length, const char* const str, const std::uint_least32_t seed = 0) {
            return murmur3c(
                murmur3b(
                    str + ((length >> 2) * MM3::chunk_size), length & 3, murmur3a(str, length >> 2, seed)
                )
                , length);
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
            std::uint_least32_t hash = seed;
            std::size_t length = 0;
            for (;; length += MM3::chunk_size) {
                if (str[length] == '\0') {
                    break;
                }
                if (str[length + 1] == '\0') {
                    hash = murmur3b(str + length, 1, hash);
                    length += 1;
                    break;
                }
                if (str[length + 2] == '\0') {
                    hash = murmur3b(str + length, 2, hash);
                    length += 2;
                    break;
                }
                if (str[length + 3] == '\0') {
                    hash = murmur3b(str + length, 3, hash);
                    length += 3;
                    break;
                }
                hash = murmur3_Chunk(str + length, hash);
            }
            return murmur3c(hash, length);
        }

    };

}

#endif // !PAX_SAPIENTICA_TYPE_MURMUR3_HPP
