#pragma once
#include <functional>
#include <string>
#include <unordered_map>
#include <memory>
#include "../Core/Deck.h"

using DeckRestrictionFunction = std::function<std::pair<bool, std::string>(std::shared_ptr<Deck>)>;

class DeckRestrictionRegistry {
public:
    static void Register(const std::string& ref, DeckRestrictionFunction func);
    static DeckRestrictionFunction Get(const std::string& ref);
};
