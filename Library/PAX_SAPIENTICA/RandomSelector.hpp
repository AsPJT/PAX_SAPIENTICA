/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023 As Project
    [Production]	2023 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_RANDOM_SELECTOR_HPP
#define PAX_SAPIENTICA_RANDOM_SELECTOR_HPP

/*##########################################################################################

##########################################################################################*/

#include <algorithm>
#include <random>
#include <stdexcept>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace paxs {

    class RandomSelector {
    public:
        explicit RandomSelector() noexcept : gen(rd()) {}

        template <typename T>
        std::vector<T> select(const std::vector<T>& vec, std::size_t num_elements) {
            if (num_elements > vec.size()) {
                throw std::invalid_argument("num_elements is greater than the size of vec");
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

        std::vector<std::pair<std::size_t, std::size_t>> select(const std::size_t num_elements_1, const std::size_t num_elements_2) {
            const std::size_t num_elements = std::min(num_elements_1, num_elements_2);

            std::vector<std::pair<std::size_t, std::size_t>> result;
            std::unordered_set<std::size_t> used_indices_1;
            std::unordered_set<std::size_t> used_indices_2;

            for (std::size_t i = 0; i < num_elements; ++i) {
                std::size_t index_1 = getRandomIndex(num_elements_1, used_indices_1);
                std::size_t index_2 = getRandomIndex(num_elements_2, used_indices_2);
                result.emplace_back(index_1, index_2);
            }

            return result;
        }

    private:
        std::random_device rd;
        std::mt19937 gen;

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

#endif // !PAX_SAPIENTICA_RANDOM_SELECTOR_HPP
