/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_CORE_UTILITY_RANDOM_HPP
#define PAX_SAPIENTICA_CORE_UTILITY_RANDOM_HPP

#include <algorithm>
#include <random>
#include <stdexcept>
#include <unordered_set>
#include <vector>

#include <PAX_SAPIENTICA/Utility/Logger.hpp>

namespace paxs {

    /// @brief Randomly select elements from a vector.
    /// @brief ランダムにベクトルから要素を選択
    class RandomSelector {
    private:
        std::mt19937& gen;
    public:
        explicit RandomSelector(std::mt19937& gen_) noexcept : gen(gen_) {}

        /// @brief Randomly select elements from a vector.
        /// @brief ベクトルから要素をランダムに選択
        /// @param vec A vector to select elements from.
        /// @param num_elements The number of elements to select.
        /// @return A vector of selected elements.
        template <typename T>
        std::vector<T> select(const std::vector<T>& vec, std::size_t num_elements) noexcept {
            if (num_elements > vec.size()) {
                PAXS_ERROR("The number of elements to select is greater than the size of the vector.");
                return {};
            }

            std::vector<T> result(vec);
            for (std::size_t i = 0; i < num_elements; ++i) {
                std::uniform_int_distribution<std::size_t> distribution(i, result.size() - 1);
                std::size_t j = distribution(gen);
                std::swap(result[i], result[j]);
            }

            result.resize(num_elements);
            return result;
        }

        /// @brief Randomly select pairs of elements from two vectors.
        /// @brief 二つの要素数が異なるベクトルからペアをランダムに選択
        /// @param result A vector of selected elements.
        /// @param num_elements_1 The number of elements to select from the first vector.
        /// @param num_elements_2 The number of elements to select from the second vector.
        void select(std::vector<std::pair<std::size_t, std::size_t>>& result,
            const std::size_t num_elements_1, const std::size_t num_elements_2) {
            const std::size_t num_elements = (std::min)(num_elements_1, num_elements_2);
            result.clear();
            std::unordered_set<std::size_t> used_indices_1;
            std::unordered_set<std::size_t> used_indices_2;

            for (std::size_t i = 0; i < num_elements; ++i) {
                std::size_t index_1 = getRandomIndex(num_elements_1, used_indices_1);
                std::size_t index_2 = getRandomIndex(num_elements_2, used_indices_2);
                result.emplace_back(index_1, index_2);
            }
        }

        /// @brief Randomly select pairs of elements from two vectors.
        /// @brief 二つの要素数が異なるベクトルからペアをランダムに選択
        /// @param num_elements_1 The number of elements to select from the first vector.
        /// @param num_elements_2 The number of elements to select from the second vector.
        /// @return A vector of selected elements.
        std::vector<std::pair<std::size_t, std::size_t>> select(const std::size_t num_elements_1, const std::size_t num_elements_2) {
            std::vector<std::pair<std::size_t, std::size_t>> result;
            select(result, num_elements_1, num_elements_2);
            return result;
        }

    private:
        std::size_t getRandomIndex(std::size_t num_elements, std::unordered_set<std::size_t>& used_indices) {
            std::uniform_int_distribution<std::size_t> distribution(0, num_elements - 1);
            std::size_t index;
            do {
                index = distribution(gen);
            } while (used_indices.find(index) != used_indices.end());
            used_indices.insert(index);
            return index;
        }
    };

}

#endif // !PAX_SAPIENTICA_CORE_UTILITY_RANDOM_HPP
