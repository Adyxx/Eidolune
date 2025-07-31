#include "SubtypeLoader.h"
#include "../Core/Subtype.h"
#include "../Http/ApiClient.h"
#include "../Registry/SubtypeRegistry.h"

#include <iostream>

std::unordered_map<int, std::shared_ptr<Subtype>> SubtypeLoader::LoadAll() {
    std::cout << "🔄 Loading Subtypes from API...\n";

    auto jsonArray = ApiClient::Get("subtypes/");
    std::unordered_map<int, std::shared_ptr<Subtype>> result;

    for (const auto& entry : jsonArray) {
        if (!entry.contains("id") || !entry.contains("name")) {
            std::cerr << "❌ Skipping subtype — missing fields: " << entry.dump() << "\n";
            continue;
        }

        int id = entry["id"];
        std::string name = entry["name"];

        auto subtype = std::make_shared<Subtype>();
        subtype->ID = id;
        subtype->Name = name;

        result[id] = subtype;
        SubtypeRegistry::Instance().Register(id, subtype);
    }

    std::cout << "✅ Loaded " << result.size() << " subtypes.\n";
    return result;
}
