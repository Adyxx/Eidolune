#include "DeckRestrictionRegistry.h"
#include <stdexcept>

DeckRestrictionRegistry& DeckRestrictionRegistry::Instance() {
    static DeckRestrictionRegistry instance;
    return instance;
}

void DeckRestrictionRegistry::Register(const std::string& id, const DeckRestriction& logic) {
    Restrictions[id] = logic;
}

DeckRestriction DeckRestrictionRegistry::Get(const std::string& id) const {
    auto it = Restrictions.find(id);
    if (it != Restrictions.end()) {
        return it->second;
    }
    throw std::runtime_error("DeckRestriction not found: " + id);
}
