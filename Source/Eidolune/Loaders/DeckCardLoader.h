#pragma once
#include <unordered_map>
#include <memory>
#include <string>
#include "../Core/DeckCard.h"

class DeckCardLoader {
public:
    static std::unordered_map<int, std::shared_ptr<DeckCard>> LoadAll();
};
