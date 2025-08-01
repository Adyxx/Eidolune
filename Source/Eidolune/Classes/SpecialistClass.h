#pragma once

#include "CharacterClass.h"
#include <memory>
#include <unordered_map>

class GameState;
class Player;
class Card;
class GameCard;
class TriggerObserver;

struct ConsumableInstance {
    std::shared_ptr<Card> SourceCard;
    int UsesRemaining = 1;

    ConsumableInstance(std::shared_ptr<Card> card, int uses)
        : SourceCard(std::move(card)), UsesRemaining(uses) {}
};

// A single slot in the consumable bag
struct ConsumableSlot {
    std::shared_ptr<ConsumableInstance> item = nullptr;
};

// The full bag (held per player)
struct ConsumableBag {
    std::vector<ConsumableSlot> slots;
    int activeSlotIndex = 0;

    ConsumableSlot* GetActiveSlot() {
        if (activeSlotIndex < 0 || activeSlotIndex >= static_cast<int>(slots.size()))
            return nullptr;
        return &slots[activeSlotIndex];
    }

    void Rotate() {
        if (slots.empty()) return;
        activeSlotIndex = (activeSlotIndex + 1) % static_cast<int>(slots.size());
    }
};

class SpecialistClass : public CharacterClass {
public:
    SpecialistClass();

    void OnTurnStart(GameState* state, Player* player) override;
    void ApplySetupBonus(Character* character);

    void UseActiveConsumable(GameState* state, Character* character, Player* player, std::shared_ptr<TriggerObserver> observer); 
    void EquipConsumable(Character* character, std::shared_ptr<Card> card, int slotIndex);
    ConsumableBag& GetBag(Character* character); 
};

