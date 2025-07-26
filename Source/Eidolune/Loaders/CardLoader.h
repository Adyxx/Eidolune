#pragma once
#include "../Core/Card.h"
#include "../Core/Trigger.h"
#include "../Core/Effect.h"
#include "../Core/CardEffectBinding.h"
#include <nlohmann/json.hpp>

#include <memory>
#include <unordered_map>
#include <string>
#include "Types.h"

class CardLoader {
public:
    static std::unordered_map<std::string, std::shared_ptr<Card>> LoadAll(const std::string& filename);

};
