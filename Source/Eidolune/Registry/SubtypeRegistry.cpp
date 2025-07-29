#include "SubtypeRegistry.h"

SubtypeRegistry& SubtypeRegistry::Instance() {
    static SubtypeRegistry instance;
    return instance;
}

void SubtypeRegistry::Register(const std::string& id, std::shared_ptr<Subtype> subtype) {
    subtypes[id] = subtype;
}

std::shared_ptr<Subtype> SubtypeRegistry::Get(const std::string& id) const {
    auto it = subtypes.find(id);
    if (it != subtypes.end()) {
        return it->second;
    }
    return nullptr;
}

void SubtypeRegistry::Clear() {
    subtypes.clear();
}
