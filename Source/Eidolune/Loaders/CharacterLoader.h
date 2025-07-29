

#pragma once
#include <unordered_map>
#include <memory>
#include <string>
#include "../Core/Character.h"

class CharacterLoader {
public:
    static std::unordered_map<int, std::shared_ptr<Character>> LoadAll();
};

