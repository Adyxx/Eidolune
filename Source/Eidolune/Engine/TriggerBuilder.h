
#pragma once

#include <functional>
#include <memory>
#include <string>

class Card;
class CardEffectBinding;

class TriggerBuilder {
public:
    static std::function<void(std::unordered_map<std::string, void*>)> Build(std::shared_ptr<CardEffectBinding> binding);
};
