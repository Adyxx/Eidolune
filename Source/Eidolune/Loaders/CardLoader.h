#pragma once
#include <unordered_map>
#include <memory>
#include "../Core/Card.h"

class CardLoader {
public:
    static std::unordered_map<int, std::shared_ptr<Card>> LoadAll();
};