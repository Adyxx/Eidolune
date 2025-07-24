

#include "TriggerBuilder.h"
#include "CardEffectBinding.h"
#include "Effect.h"
#include "GameCard.h"
#include "Trigger.h"

std::function<void(std::unordered_map<std::string, void*>)>
TriggerBuilder::Build(std::shared_ptr<CardEffectBinding> binding) {
    return [binding](std::unordered_map<std::string, void*> ctx) {
        auto eventGameCardPtr = binding->GetEventGameCard();
        if (!eventGameCardPtr) {
            throw std::runtime_error("No event GameCard set in binding");
        }
        GameCard* eventCard = eventGameCardPtr.get();
        GameCard* triggerCard = static_cast<GameCard*>(ctx.at("source"));

        std::string triggerCode = binding->GetTrigger()->ScriptReference;

        if (triggerCode == "on_death") {
            if (triggerCard != eventCard) return;
        }
        if (triggerCode == "on_friendly_death") {
            if (triggerCard == eventCard) return;
            if (triggerCard->Owner != eventCard->Owner) return;
        }

        auto effect = binding->GetEffect();
        effect->GetExecutable()(eventCard, triggerCard, binding->GetValue().value_or(0));
    };
}



    /*
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
    */