#pragma once
#include <string>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class ApiClient {
public:
    static json Get(const std::string& endpoint);
};
