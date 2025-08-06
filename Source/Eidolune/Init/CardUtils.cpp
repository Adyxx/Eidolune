
#include "CardUtils.h"
#include "Player.h"

namespace CardUtils {

void SubscribeCardTriggers(std::shared_ptr<GameCard> card, std::shared_ptr<TriggerObserver> observer) {

    if (card->Model->EffectBindings.empty()) {
        return;
    }

    for (const auto& originalBinding : card->Model->EffectBindings) {

        auto binding = std::make_shared<CardEffectBinding>(*originalBinding);

        if (!binding) {
            std::cout << "❌ Failed to copy original binding.\n";
            continue;
        }

        auto triggerPtr = binding->GetTrigger();
        if (!triggerPtr) {
            std::cout << "⚠️ Skipping binding: no trigger found.\n";
            continue;
        }

        std::string triggerCode = triggerPtr->ScriptReference;

        auto* triggerMeta = TriggerRegistry::Instance().GetInfo(triggerCode);
        if (!triggerMeta) {
            std::cout << "❌ No trigger metadata found for code: " << triggerCode << "\n";
            continue;
        }

        std::string eventName = triggerMeta->Event;

        binding->EventGameCard = card;

        std::function<void(const std::unordered_map<std::string, void*>)> effectToRegister;
        auto builder = TriggerBuilder::Build(binding);

        auto conditionPtr = binding->GetCondition();
        if (conditionPtr) {
            std::string conditionCode = conditionPtr->ToString();

            effectToRegister = [=](const std::unordered_map<std::string, void*>& ctx) {
                if (ConditionEvaluator::Evaluate(conditionCode, card, binding->GetValue().value_or(0))) {

                    builder(ctx);
                } 
            };
        } else {
            effectToRegister = builder;
        }

        observer->Subscribe(eventName, effectToRegister);
    }

}


std::shared_ptr<GameCard> RegisterCardMidGame(std::shared_ptr<Card> templateCard, Player* owner, CardZone zone, std::shared_ptr<TriggerObserver> observer) {

    if (!templateCard || !owner) {
        std::cerr << "❌ Invalid input to RegisterCardMidGame.\n";
        return nullptr;
    }

    auto gameCard = std::make_shared<GameCard>(templateCard);
    gameCard->Owner = owner;
    gameCard->Zone = zone;

    CardUtils::SubscribeCardTriggers(gameCard, observer);

    switch (zone) {
        case CardZone::DECK:
            owner->DrawPile.push_back(gameCard);
            std::cout << "📥 Card '" << gameCard->GetName() << "' added to DECK.\n";
            break;
        case CardZone::HAND:
            owner->Hand.push_back(gameCard);
            std::cout << "✋ Card '" << gameCard->GetName() << "' added to HAND.\n";
            break;
        case CardZone::BOARD:
            owner->Board.push_back(gameCard);
            std::cout << "⚔️ Card '" << gameCard->GetName() << "' added to BOARD.\n";
            break;
        case CardZone::GRAVEYARD:
            owner->Graveyard.push_back(gameCard);
            std::cout << "☠️ Card '" << gameCard->GetName() << "' added to GRAVEYARD.\n";
            break;
        case CardZone::EXILE:
            owner->ExileZone.push_back(gameCard);
            std::cout << "🚫 Card '" << gameCard->GetName() << "' added to EXILE.\n";
            break;
        case CardZone::OATHZONE:
            owner->OathZone.push_back(gameCard);
            std::cout << "📜 Card '" << gameCard->GetName() << "' added to OATHZONE.\n";
            break;
        case CardZone::CONSUMABLE_BAG:
            std::cout << "🧪 CONSUMABLE_BAG is managed separately. Card created but not stored here.\n";
            break;
        default:
            std::cerr << "❓ Unknown CardZone. Card '" << gameCard->GetName() << "' not added to any zone.\n";
            break;
    }

    return gameCard;
}



}