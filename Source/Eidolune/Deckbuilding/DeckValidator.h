#pragma once
#include <string>
#include <vector>
#include <memory>
#include "../Core/Deck.h"

struct DeckValidationResult {
    bool IsPlayable = true;
    std::vector<std::string> Issues;
};

class DeckValidator {
public:
    static DeckValidationResult Validate(std::shared_ptr<Deck> deck);
};
