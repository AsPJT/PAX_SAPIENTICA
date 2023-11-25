/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023 As Project
    [Production]	2023 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_UNIQUE_IDENTIFICATION_HPP
#define PAX_SAPIENTICA_UNIQUE_IDENTIFICATION_HPP

/*##########################################################################################

##########################################################################################*/

#include <PAX_SAPIENTICA/Logger.hpp>

namespace paxs {

    /// @brief A class that generates a unique id.
    /// @brief ユニークなIDを生成するクラス。
    template <typename IdType>
    class UniqueIdentification {
    private:
        static IdType counter;
    public:
        /// @brief Generate a unique id.
        /// @brief ユニークなIDを生成する。
        static IdType generate() {
            if (counter == std::numeric_limits<IdType>::max()) {
                Logger logger("Save/error_log.txt");
                std::string message = "The id has reached the maximum value of the type.";
                logger.log(Logger::Level::PAX_WARNING, __FILE__, __LINE__, message);
                reset();
            }
            return counter++;
        }

        /// @brief Reset the counter.
        /// @brief カウンターをリセットする。
        static void reset() {
            counter = 1;
        }
    };

    template <typename IdType>
    IdType UniqueIdentification<IdType>::counter = 1;

}

#endif // !PAX_SAPIENTICA_UNIQUE_IDENTIFICATION_HPP
