

#include "TriggerBuilder.h"
#include "CardEffectBinding.h"
#include "Effect.h"

std::function<void(std::unordered_map<std::string, void*>)>
TriggerBuilder::Build(std::shared_ptr<CardEffectBinding> binding) {
    return [binding](std::unordered_map<std::string, void*> context) {
        if (binding && binding->GetEffect()) {
            auto source = binding->GetEventCard();
            auto target = context["target"];
            auto value = binding->GetValue();

            binding->GetEffect()->GetExecutable()(static_cast<void*>(source.get()), target, value);
        } else {
            throw std::runtime_error("Invalid binding or effect");
        }
    };
}
