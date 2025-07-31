#include "DeckRestrictionRegistry.h"

static std::unordered_map<std::string, DeckRestrictionFunction> registry;

void DeckRestrictionRegistry::Register(const std::string& ref, DeckRestrictionFunction func) {
    registry[ref] = func;
}

DeckRestrictionFunction DeckRestrictionRegistry::Get(const std::string& ref) {
    auto it = registry.find(ref);
    return (it != registry.end()) ? it->second : nullptr;
}
