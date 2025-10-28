/*##########################################################################################

	PAX SAPIENTICA Library 💀🌿🌏

	[Planning]		2023-2024 As Project
	[Production]	2023-2024 As Project
	[Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
	[License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_KEY_LANGUAGE_KEYS_HPP
#define PAX_SAPIENTICA_KEY_LANGUAGE_KEYS_HPP

#include <array>
#include <cstdint>

#include <PAX_SAPIENTICA/MurMur3.hpp>

namespace paxs {

    /// @brief 言語キーの文字列定数
    namespace LanguageKeys {
        constexpr const char* EN_US = "en-US";
        constexpr const char* JA_JP = "ja-JP";
        constexpr const char* ZH_TW = "zh-TW";
        constexpr const char* ZH_CN = "zh-CN";
        constexpr const char* KO_KR = "ko-KR";
        constexpr const char* HIRAGANA = "hiragana";
        constexpr const char* ES_ES = "es-ES";
        constexpr const char* PT_BR = "pt-BR";
        constexpr const char* DE_DE = "de-DE";
        constexpr const char* FR_FR = "fr-FR";
        constexpr const char* IT_IT = "it-IT";
        constexpr const char* TR_TR = "tr-TR";
        constexpr const char* PL_PL = "pl-PL";
        constexpr const char* EL_GR = "el-GR";
        constexpr const char* NL_NL = "nl-NL";
        constexpr const char* CS_CZ = "cs-CZ";
        constexpr const char* UK_UA = "uk-UA";
        constexpr const char* RU_RU = "ru-RU";
        constexpr const char* ID_ID = "id-ID";
        constexpr const char* FA_IR = "fa-IR";
        constexpr const char* AR_SA = "ar-SA";
        constexpr const char* AIN = "ain";

        /// @brief 全言語キーのハッシュ化済み配列
        inline constexpr std::array ALL_LANGUAGE_HASHES = {
            MurMur3::calcHash(EN_US),
            MurMur3::calcHash(JA_JP),
            MurMur3::calcHash(ZH_TW),
            MurMur3::calcHash(ZH_CN),
            MurMur3::calcHash(KO_KR),
            MurMur3::calcHash(HIRAGANA),
            MurMur3::calcHash(ES_ES),
            MurMur3::calcHash(PT_BR),
            MurMur3::calcHash(DE_DE),
            MurMur3::calcHash(FR_FR),
            MurMur3::calcHash(IT_IT),
            MurMur3::calcHash(TR_TR),
            MurMur3::calcHash(PL_PL),
            MurMur3::calcHash(EL_GR),
            MurMur3::calcHash(NL_NL),
            MurMur3::calcHash(CS_CZ),
            MurMur3::calcHash(UK_UA),
            MurMur3::calcHash(RU_RU),
            MurMur3::calcHash(ID_ID),
            MurMur3::calcHash(FA_IR),
            MurMur3::calcHash(AR_SA),
            MurMur3::calcHash(AIN)
        };
    }

}

#endif // !PAX_SAPIENTICA_KEY_LANGUAGE_KEYS_HPP
