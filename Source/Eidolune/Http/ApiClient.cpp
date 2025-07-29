#include "ApiClient.h"
#include <cpr/cpr.h>
#include <iostream>

constexpr const char* BASE_URL = "http://127.0.0.1:8000/api/";

json ApiClient::Get(const std::string& endpoint) {
    std::string url = BASE_URL + endpoint;
    auto response = cpr::Get(cpr::Url{url});

    if (response.status_code != 200) {
        std::cerr << "❌ Failed to fetch " << url << " — HTTP " << response.status_code << "\n";
        return {};
    }

    return json::parse(response.text);
}
