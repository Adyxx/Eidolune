#pragma once
#include <unordered_map>
#include <string>
#include <memory>
#include "../Core/Deck.h"
#include "../Data/UserData.h"

class DeckLoader {
public:
    static std::unordered_map<int, std::shared_ptr<Deck>> LoadAll();
};
