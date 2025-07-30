#pragma once
#include <unordered_map>
#include <memory>
#include <string>
#include "../Registry/EffectRegistry.h"

class Effect;

class EffectLoader {
public:
    static std::unordered_map<std::string, std::shared_ptr<Effect>> LoadAll();
};
