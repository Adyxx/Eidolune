#include "ConditionLoader.h"
#include "../Http/ApiClient.h"
#include "../Registry/ConditionRegistry.h"
#include <iostream>

std::unordered_map<std::string, std::shared_ptr<Condition>> ConditionLoader::LoadAll() {
    std::cout << "🔄 Loading Conditions from API...\n";
    std::unordered_map<std::string, std::shared_ptr<Condition>> result;

    json conditionsJson = ApiClient::Get("conditions/");

    for (const auto& c : conditionsJson) {
        if (!c.contains("id") || !c.contains("script_reference") || !c.contains("name")) {
            std::cerr << "❌ Condition missing fields: " << c.dump() << "\n";
            continue;
        }

        int id = c["id"];
        std::string ref = c["script_reference"];

        try {
            auto condition = std::make_shared<Condition>(c);
            ConditionRegistry::Instance().Register(id, ref, condition);
            result[ref] = condition;
        } catch (const std::exception& ex) {
            std::cerr << "❌ Failed to parse Condition (" << ref << "): " << ex.what() << "\n";
            continue;
        }
    }

    std::cout << "✅ Loaded " << result.size() << " conditions.\n";
    return result;
}