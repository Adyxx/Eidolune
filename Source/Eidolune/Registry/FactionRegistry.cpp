#include "FactionRegistry.h"

FactionRegistry& FactionRegistry::Instance() {
    static FactionRegistry instance;
    return instance;
}

void FactionRegistry::Register(int id, std::shared_ptr<Faction> faction) {
    if (faction) {
        factions[id] = faction;
    }
}

std::shared_ptr<Faction> FactionRegistry::Get(int id) const {
    auto it = factions.find(id);
    if (it != factions.end()) {
        return it->second;
    }
    return nullptr;
}

void FactionRegistry::Clear() {
    factions.clear();
}
