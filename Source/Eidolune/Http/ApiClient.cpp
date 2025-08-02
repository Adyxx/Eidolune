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

json ApiClient::Post(const std::string& endpoint, const json& data) {
    std::string url = BASE_URL + endpoint;
    auto response = cpr::Post(
        cpr::Url{url},
        cpr::Header{{"Content-Type", "application/json"}},
        cpr::Body{data.dump()}
    );

    if (response.status_code != 200) {
        std::cerr << "❌ Failed to post to " << url << " — HTTP " << response.status_code << "\n";
        return {};
    }

    return json::parse(response.text);
}
