
#pragma once

#include <memory>

class CardEffectBinding;

struct EffectCallContext {
    std::shared_ptr<CardEffectBinding> CurrentBinding = nullptr;
};

EffectCallContext& GetEffectCallContext();
