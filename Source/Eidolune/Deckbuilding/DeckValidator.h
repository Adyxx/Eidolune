#pragma once
#include <string>
#include <vector>
#include <memory>

class Deck;

struct DeckValidationResult {
    bool IsPlayable = true;
    std::vector<std::string> Issues;
};

class DeckValidator {
public:
    static DeckValidationResult Validate(std::shared_ptr<Deck> deck);
};
