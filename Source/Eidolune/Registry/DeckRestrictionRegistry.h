#pragma once
#include <functional>
#include <string>
#include "Deck.h"

using DeckRestrictionFn = std::function<bool(const Deck& deck)>;


struct DeckRestriction {
    DeckRestrictionFn Logic;
    std::string Description;
};

class DeckRestrictionRegistry {
public:
    static DeckRestrictionRegistry& Instance();
    void Register(const std::string& id, const DeckRestriction& logic);
    DeckRestriction Get(const std::string& id) const;
private:
    std::unordered_map<std::string, DeckRestriction> Restrictions;

};

