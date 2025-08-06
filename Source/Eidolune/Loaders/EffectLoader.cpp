#include "EffectLoader.h"
#include "../Http/ApiClient.h"
#include <iostream>

std::unordered_map<std::string, std::shared_ptr<Effect>> EffectLoader::LoadAll() {
    std::cout << "🔄 Loading Effects from API...\n";
    std::unordered_map<std::string, std::shared_ptr<Effect>> result;

    json effectsJson = ApiClient::Get("effects/");

    for (const auto& e : effectsJson) {
        if (!e.contains("id") || !e.contains("script_reference") || !e.contains("name")) {
            std::cerr << "❌ Effect missing 'id', 'script_reference', or 'name': " << e.dump() << "\n";
            continue;
        }

        int id = e["id"].get<int>();
        std::string ref = e["script_reference"].get<std::string>();
        std::string name = e["name"].get<std::string>();

        std::cout << "🔹 Parsing Effect: " << name << " [" << ref << "] (ID: " << id << ")\n";

        try {
            auto effect = std::make_shared<Effect>(e);

            // ✅ Attach function if available
            auto funcEffect = EffectRegistry::Instance().Get(ref);
            if (funcEffect && funcEffect->GetExecutable()) {
                std::cout << "🔗 Linked executable function for: " << ref << "\n";
                effect->Executable = funcEffect->GetExecutable();
            } else {
                std::cerr << "⚠️ No function registered for effect: " << ref << "\n";
            }

            EffectRegistry::Instance().Register(id, ref, effect);
            result[ref] = effect;
        } catch (const std::exception& ex) {
            std::cerr << "❌ Failed to parse Effect (" << ref << "): " << ex.what() << "\n";
            continue;
        }
    }

    std::cout << "✅ Loaded " << result.size() << " effects.\n";
    return result;
}