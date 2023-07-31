/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023 As Project
    [Production]	2023 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_HTTP_CLIENT_HPP
#define PAX_SAPIENTICA_HTTP_CLIENT_HPP

/*##########################################################################################

##########################################################################################*/

#include <httplib.h>

namespace paxs {

    /// @brief A class that sends HTTP requests.
    /// @brief HTTPリクエストを送信するクラス
    struct HttpClient {
        static std::string get(const std::string& baseUrl, const std::string& path) {
            httplib::Client client(baseUrl);
            auto res = client.Get(path.c_str());
            if (res && res->status == 200) {
                return res->body;
            } else {
                return "error";
            }
        }

        static std::string post(const std::string& baseUrl, const std::string& path, 
            const std::string& body, const std::string& contentType) 
        {
            httplib::Client client(baseUrl);
            auto res = client.Post(path.c_str(), body, contentType.c_str());
            if (res && res->status == 200) {
                return res->body;
            } else {
                return "error";
            }
        }
    };

}

#endif // !PAX_SAPIENTICA_HTTP_CLIENT_HPP