#include "Player.h"
#include <iostream>
#include "../Definitions/EffectDefinitions.h"
#include "Target.h"

Player::Player(std::shared_ptr<Deck> deck, int index)
    : DeckRef(deck), PlayerIndex(index) {}

std::string Player::GetName() const {
    return "Player " + std::to_string(PlayerIndex + 1);
}

void Player::StartTurn() {
    std::cout << "▶️ " << GetName() << "'s turn begins.\n";
    if (MaxEnergy < 10) MaxEnergy++;
    Energy = MaxEnergy;
    //DrawCard(nullptr, this, 1);
    DrawCard(nullptr, Target::FromPlayer(this), 1);

}

void Player::EndTurn() {
    std::cout << "⏹ " << GetName() << "'s turn ends.\n";
    if ((int)Hand.size() > MaxHandSize) {
        std::cout << "🗑️ You have too many cards. (not implemented: discard)\n";
    }

    for (auto& card : Board) {
        if (card) {
            card->Tapped = false;
            card->SummoningSickness = false;
        }
    }
}

