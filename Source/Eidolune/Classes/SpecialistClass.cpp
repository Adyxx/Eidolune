#include "SpecialistClass.h"
#include "GameState.h"
#include "Player.h"
#include "Card.h"
#include "GameCard.h"
#include "GameActions.h"
#include "TriggerObserver.h"
#include "CardEffectBinding.h"
#include "TriggerBuilder.h"
#include "TriggerRegistry.h"
#include "ConditionEvaluator.h"

#include <iostream>

extern void SubscribeCardTriggers(std::shared_ptr<GameCard> card, std::shared_ptr<TriggerObserver> observer);

static std::unordered_map<Character*, ConsumableBag> bags;

SpecialistClass::SpecialistClass() {
    Name = "Specialist";
    Description = "Bring a bag of consumables to the game. Combine ingredients and interact with the bag.";
}

void SpecialistClass::OnTurnStart(GameState* state, Player* player) {
    // Optional future rotation:
    // bags[player->ID].Rotate();
}

ConsumableBag& SpecialistClass::GetBag(Character* character) {
    auto& bag = bags[character];
    if (bag.slots.empty()) bag.slots.resize(3);
    return bags[character];
}


void SpecialistClass::ApplySetupBonus(Character* character) {
    auto& bag = bags[character];
    bag.slots.resize(3);
    std::cout << "🎒 Specialist bag initialized with 3 slots for character.\n";
}


void SpecialistClass::UseActiveConsumable(GameState* state, Character* character, Player* player, std::shared_ptr<TriggerObserver> observer) {
    auto& bag = bags[character];
    auto* slot = bag.GetActiveSlot();

    if (!slot || !slot->item || !slot->item->SourceCard) {
        std::cout << "⚠️ No consumable in active slot.\n";
        return;
    }

    auto cardData = slot->item->SourceCard;
    auto gameCard = std::make_shared<GameCard>(cardData);

    gameCard->Owner = player;
    gameCard->Zone = CardZone::CONSUMABLE_BAG;

    SubscribeCardTriggers(gameCard, observer);

    if (player->Energy < gameCard->GetCost()) {
        std::cout << "🚫 Not enough energy to use consumable: " << gameCard->GetName() << "\n";
        return;
    }

    std::cout << "🧪 Using consumable: " << gameCard->GetName() << "\n";
    GameActions::PlayCardDirect(gameCard, observer);

    if (--slot->item->UsesRemaining <= 0) {
        slot->item = nullptr;
        std::cout << "💥 Consumable depleted and removed from bag.\n";
    }
}


void SpecialistClass::EquipConsumable(Character* character, std::shared_ptr<Card> card, int slotIndex) {
    auto& bag = bags[character];

    if (slotIndex < 0 || slotIndex >= static_cast<int>(bag.slots.size())) {
        std::cerr << "❌ Invalid slot index for Specialist bag.\n";
        return;
    }

    if (!card) {
        bag.slots[slotIndex].item = nullptr;
        std::cout << "🧹 Slot " << slotIndex << " unequipped.\n";
        return;
    }

    int defaultUses = (card->AuxilaryType == AuxiliaryCardType::CONSUMABLE) ? 1 : 0;
    auto instance = std::make_shared<ConsumableInstance>(card, defaultUses);
    bag.slots[slotIndex].item = instance;

    std::cout << "🎒 Equipped '" << card->Name << "' to slot " << slotIndex << "\n";
}
