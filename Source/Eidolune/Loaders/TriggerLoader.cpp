#include "TriggerLoader.h"
#include "../Http/ApiClient.h"
#include "../Registry/TriggerRegistry.h"
#include "../Core/Trigger.h"
#include <iostream>

std::unordered_map<std::string, std::shared_ptr<Trigger>> TriggerLoader::LoadAll() {
    std::cout << "🔄 Loading Triggers from API...\n";
    std::unordered_map<std::string, std::shared_ptr<Trigger>> result;

    json triggersJson = ApiClient::Get("triggers/");

    for (const auto& t : triggersJson) {
        if (!t.contains("id") || !t.contains("script_reference") || !t.contains("description")) {
            std::cerr << "❌ Trigger missing fields: " << t.dump() << "\n";
            continue;
        }

        int id = t["id"];
        std::string ref = t["script_reference"];

        try {
            auto trigger = std::make_shared<Trigger>(t);
            TriggerRegistry::Instance().RegisterShared(id, trigger);
            result[ref] = trigger;

            TriggerInfo info;
            info.Name = ref;
            info.Description = trigger->Description;
            info.Event = trigger->Event;
            info.Builder = {};

            TriggerRegistry::Instance().RegisterInfo(ref, info); 

        } catch (const std::exception& ex) {
            std::cerr << "❌ Failed to parse Trigger (" << ref << "): " << ex.what() << "\n";
            continue;
        }
    }

    std::cout << "✅ Loaded " << result.size() << " triggers.\n";
    return result;
}
