#include "GameCard.h"
#include "Player.h"
#include <iostream>

#include <atomic>
static std::atomic<int> GlobalGameCardId = 0;

GameCard::GameCard(std::shared_ptr<Card> model)
    : Model(model) {
        Id = GlobalGameCardId++;
         //std::cout << "Created GameCard with ID: " << Id << " and model name: " << model->Name << "\n";
    }

std::string GameCard::GetName() const {
    return Model ? Model->Name : "";
}

int GameCard::GetCost() const {
    return Model ? Model->Cost : 0;
}

std::optional<int> GameCard::GetPower() const {
    return Model ? Model->Power : std::nullopt;
}

std::optional<int> GameCard::GetHealth() const {
    return Model ? Model->Health : std::nullopt;
}

int GameCard::CurrentHealth() const {
    if (!Model || !Model->Health.has_value()) return 0;
    //return Model->Health.value() - DamageTaken;
    return MaxHealth() - DamageTaken;
}

std::string GameCard::ToString() const {
    std::string output = "<GameCard ";
    output += GetName();
    output += " (Cost: " + std::to_string(GetCost());
    if (GetPower()) {
        output += ", Power: " + std::to_string(GetPower().value());
    }
    if (GetHealth()) {
        output += ", Health: " + std::to_string(CurrentHealth());
    }
    output += ")>";
    return output;
}

///////////////////////////////////////////////////////////


int GameCard::BaseAttack() const {
    return Model && Model->Power.has_value() ? Model->Power.value() : 0;
}

int GameCard::BaseHealth() const {
    return Model && Model->Health.has_value() ? Model->Health.value() : 0;
}

int GameCard::CurrentAttack() const {
    int total = BaseAttack();
    for (const auto& [_, aura] : ActiveAuras) total += aura.AttackBuff;
    return total;
}

int GameCard::MaxHealth() const {
    int total = BaseHealth();
    for (const auto& [_, aura] : ActiveAuras) total += aura.HealthBuff;
    return total;
}

void GameCard::ApplyAura(int sourceId, int atkBuff, int hpBuff) {
    if (ActiveAuras.count(sourceId) > 0) return; // already applied
    ActiveAuras[sourceId] = { atkBuff, hpBuff };

    // Reduce damage if HP increased (doesn't heal — just gives room)
    // Do NOT adjust DamageTaken — damage stays unless healed
}

void GameCard::RemoveAura(int sourceId) {
    auto it = ActiveAuras.find(sourceId);
    if (it == ActiveAuras.end()) return;

    auto aura = it->second;

    int oldMax = MaxHealth();
    int currentHP = oldMax - DamageTaken;

    ActiveAuras.erase(it); // aura removed

    int newMax = MaxHealth();
    int auraLoss = oldMax - newMax;

    // Calculate new current HP by subtracting auraLoss (if possible)
    int newCurrentHP = currentHP;

    if (currentHP > auraLoss) {
        newCurrentHP = currentHP - auraLoss;
    } else {
        // prevent death from aura loss, keep current HP same
        newCurrentHP = currentHP;
    }

    // Damage taken recalculated by formula: damageTaken = newMax - newCurrentHP
    DamageTaken = newMax - newCurrentHP;

if (DamageTaken < 0) DamageTaken = 0;
    std:: cout << "oldMax: " << oldMax << ", oldCurrent: " << currentHP << " ,newMax:" << newMax << ", newCurrent: " << newCurrentHP <<", damageTaken" << DamageTaken << "\n";
    std::cout << "🔻 Removed aura from " << GetName()
              << " (-" << aura.AttackBuff << "/-" << aura.HealthBuff << ")\n";
}
