

/*
For directly modifying the numbers and statuses on existing cards.
Covers:

Deal damage
Heal
Buff/nerf stats
Apply or remove status effects (poison, freeze, shield, etc.)
Aura recalculations
Temporary or permanent modifiers

*/

#include "GameActions.h"
#include "Target.h"
#include "Trigger.h"
#include <optional>
#include "EffectRegistry.h"

void DealDamage(void* source, Target target, std::optional<int> value) {
    int damage = value.value_or(1);
    std::cout << "[Effect] DealDamage for " << damage << "\n";

    GameActions::ResolveDamage(source, target, damage, "effect");
}


void Aura_OverallStatBoost(void* source, Target, std::optional<int> value) {
    if (!source) return;

    auto* sourceCard = static_cast<GameCard*>(source);
    if (!sourceCard || !sourceCard->Owner) return;

    int buffAmount = value.value_or(1);

    for (int row = 0; row < Player::BoardHeight; ++row) {
        for (int col = 0; col < Player::BoardWidth; ++col) {
            auto& slot = sourceCard->Owner->GridBoard[row][col];
            if (!slot) continue;

            auto& card = slot;

            // Avoid reapplying the same aura multiple times
            if (card->ActiveAuras.count(sourceCard->Id)) {
                continue;
            }

            card->ApplyAura(sourceCard->Id, buffAmount, buffAmount);

            std::cout << "🟢 Aura from [" << sourceCard->GetName()
                      << "] applied to [" << card->GetName()
                      << "] (+" << buffAmount << "/+" << buffAmount << ")\n";
        }
    }
}



void GetTimeCounter(void* source, Target, std::optional<int> value) {
    auto* card = static_cast<GameCard*>(source);
    if (card && value) {
        card->TimeCounter += value.value();
        std::cout << "⏳ Gained " << value.value() << " time counters.\n";
    }
}

void RemoveTimeCounter(void* source, Target, std::optional<int> value) {
    auto* card = static_cast<GameCard*>(source);
    if (card && value) {
        card->TimeCounter -= value.value();
        std::cout << "⌛ Removed " << value.value() << " time counters. Now: " << card->TimeCounter << "\n";
    }
}

void ChooseBasedOnTimeCounter(void* source, Target target, std::optional<int>) {
    auto* card = static_cast<GameCard*>(source);
    if (!card || !card->Model) return;

    int time = card->TimeCounter;
    std::string prefix = "time_" + std::to_string(time) + "_opt";

    std::vector<std::shared_ptr<CardEffectBinding>> matchingBindings;

    for (const auto& binding : card->Model->EffectBindings) {
        if (!binding || !binding->GetTrigger()) continue;

        const std::string& triggerName = binding->GetTrigger()->GetName();
        if (triggerName.rfind(prefix, 0) == 0) { // starts with prefix
            matchingBindings.push_back(binding);
        }
    }

    if (matchingBindings.empty()) {
        std::cout << "❌ No choice options for time_" << time << "\n";
        return;
    }

    std::shared_ptr<CardEffectBinding> chosenBinding;

    if (matchingBindings.size() == 1) {
        chosenBinding = matchingBindings[0];
    } else {
        std::vector<std::string> optionLabels;
        for (const auto& b : matchingBindings) {
            optionLabels.push_back(b->GetTrigger()->GetName()); // or use b->ToString() if better
        }

        int choiceIndex = card->Owner->PromptChooseOption(optionLabels);
        if (choiceIndex < 0 || choiceIndex >= static_cast<int>(matchingBindings.size())) {
            std::cout << "❌ Invalid choice index.\n";
            return;
        }

        chosenBinding = matchingBindings[choiceIndex];
    }

    std::string chosenEvent = chosenBinding->GetTrigger()->GetName();

    std::cout << "📤 Emitting trigger: " << chosenEvent << "\n";
    card->Owner->Observer->Emit(chosenEvent, {
        { "source", card },
        { "owner", card->Owner }
    });
}




void ModifyAttack(void* source, Target target, std::optional<int> value) {
    auto* card = static_cast<GameCard*>(source);
    if (!card || !value) return;

    int delta = value.value();
    // Use a special source ID for direct stat changes if needed
    int sourceId = 0; 
    card->ApplyAura(sourceId, delta, 0);

    std::cout << "🗡️ " << card->GetName() << " attack modified by "
              << delta << " → " << card->CurrentAttack() << "\n";
}

void ModifyMaxHealth(void* source, Target target, std::optional<int> value) {
    auto* card = static_cast<GameCard*>(source);
    if (!card || !value) return;

    int delta = value.value();
    int sourceId = 0;
    card->ApplyAura(sourceId, 0, delta);

    // Clamp DamageTaken if max health decreased
    if (delta < 0 && card->CurrentHealth() < card->DamageTaken) {
        card->DamageTaken = card->CurrentHealth() - 1;
    }

    std::cout << "❤️ " << card->GetName() << " max health modified by "
              << delta << " → " << card->MaxHealth() << "\n";
}

void ModifyCost(void* source, Target target, std::optional<int> value) {
    auto* card = static_cast<GameCard*>(source);
    if (!card || !value) return;

    int delta = value.value();
    int sourceId = 0;
    card->ApplyCostModifier(sourceId, delta);

    std::cout << "💰 " << card->GetName() << " cost modified by "
              << delta << " → " << card->CurrentCost() << "\n";
}

void HealHero(void* source, Target target, std::optional<int> value) {
    if (target.type != Target::Type::PLAYER || !value) return;

    auto* player = static_cast<Player*>(target.ptr);
    int amount = value.value();

    player->Health = std::min(player->MaxHealth, player->Health + amount);

    //at least for now, it is here
    if (player->HasClassBloodbound) {
        player->BloodboundData->BloodEcho += amount; // gained HP
    }
    //////////////
    
    std::cout << "💖 " << player->GetName() << " healed " << amount
              << " → " << player->Health << "/" << player->MaxHealth << "\n";
}


void RegisterEffectFunctions_StatAndStatus() {
    std::cout << "📦 Registering StatAndStatus effects...\n";

    EffectRegistry::Instance().Register("deal_damage", DealDamage);
    EffectRegistry::Instance().Register("Aura_OverallStatBoost", Aura_OverallStatBoost);
    EffectRegistry::Instance().Register("get_time_counter", GetTimeCounter);
    EffectRegistry::Instance().Register("remove_time_counter", RemoveTimeCounter);
    EffectRegistry::Instance().Register("choose_on_time_counter", ChooseBasedOnTimeCounter);



    EffectRegistry::Instance().Register("modify_attack", ModifyAttack);
    EffectRegistry::Instance().Register("modify_maxhealth", ModifyMaxHealth);
    EffectRegistry::Instance().Register("modify_cost", ModifyCost);
    EffectRegistry::Instance().Register("heal_hero", HealHero);

}