
#include "CardUtils.h"
#include "Player.h"

namespace CardUtils {
/*
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
                if (ConditionEvaluator::Evaluate(conditionCode, card, binding->GetConditionValue().value_or(0))) {

                    builder(ctx);
                } 
            };
        } else {
            effectToRegister = builder;
        }
        //std::cout << card->ToString() << "  " << eventName << "\n";
        observer->Subscribe(eventName, effectToRegister);
    }

}
*/

void SubscribeCardTriggers(std::shared_ptr<GameCard> card, std::shared_ptr<TriggerObserver> observer) {
    // Nothing to do if there are no static or runtime bindings
    if (card->Model->EffectBindings.empty() && card->RuntimeEffectBindings.empty()) {
        return;
    }

    // Helper lambda to subscribe a list of bindings
    auto subscribeBindings = [&](const std::vector<std::shared_ptr<CardEffectBinding>>& bindings) {
        for (const auto& originalBinding : bindings) {

            if (!originalBinding) {
                std::cout << "❌ Skipping null binding.\n";
                continue;
            }

            // Copy so we don't mutate the original
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

            // Store reference to the actual GameCard instance
            binding->EventGameCard = card;

            // Build effect executor
            auto builder = TriggerBuilder::Build(binding);

            // Optional condition check
            std::function<void(const std::unordered_map<std::string, void*>)> effectToRegister;
            if (auto conditionPtr = binding->GetCondition()) {
                std::string conditionCode = conditionPtr->ToString();
                effectToRegister = [=](const std::unordered_map<std::string, void*>& ctx) {
                    if (ConditionEvaluator::Evaluate(conditionCode, card, binding->GetConditionValue().value_or(0))) {
                        builder(ctx);
                    }
                };
            } else {
                effectToRegister = builder;
            }

            observer->Subscribe(eventName, effectToRegister);
        }
    };

    // Subscribe static model bindings
    subscribeBindings(card->Model->EffectBindings);

    // Subscribe runtime (temporary) bindings
    subscribeBindings(card->RuntimeEffectBindings);
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
        case CardZone::BOARD: {
            std::vector<std::pair<int,int>> freeSlots;

            for (int r = 0; r < 2; ++r)
                for (int c = 0; c < 7; ++c)
                    if (!owner->GridBoard[r][c])
                        freeSlots.push_back({r, c});

            if (!freeSlots.empty()) {
                auto [row, col] = freeSlots[rand() % freeSlots.size()];
                owner->GridBoard[row][col] = gameCard;
                std::cout << "⚔️ Card '" << gameCard->GetName()
                        << "' placed randomly at (" << row << "," << col << ").\n";
            } else {
                std::cout << "🚫 No space on BOARD for '" << gameCard->GetName() << "'.\n";
            }
            break;
        }

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