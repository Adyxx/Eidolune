#pragma once
#include <unordered_map>
#include <memory>
#include <string>
#include "../Core/CardEffectBinding.h"

class CardEffectBindingLoader {
public:
    static std::vector<std::shared_ptr<CardEffectBinding>> LoadAll();
};
