#include "SubtypeRegistry.h"

SubtypeRegistry& SubtypeRegistry::Instance() {
    static SubtypeRegistry instance;
    return instance;
}

void SubtypeRegistry::Register(int id, std::shared_ptr<Subtype> subtype) {
    registry[id] = subtype;
}

std::shared_ptr<Subtype> SubtypeRegistry::Get(int id) const {
    auto it = registry.find(id);
    if (it != registry.end()) {
        return it->second;
    }
    return nullptr;
}

void SubtypeRegistry::Clear() {
    registry.clear();
}
