/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023 As Project
    [Production]	2023 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#include <gtest/gtest.h>

#include <PAX_SAPIENTICA/HttpClient.hpp>

TEST (HttpClientUnitTest, get) {
    const std::string url = "http://example.com";
    
    const std::string no_expected = "error";

    const std::string actual = paxs::HttpClient::get(url, "/");

    ASSERT_NE(actual, no_expected);
}