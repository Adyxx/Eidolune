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
    TurnCount++;
    CardsPlayedThisTurn = 0;
    std::cout << "▶️ " << GetName() << "'s turn begins.\n";
    if (MaxEnergy < 10) MaxEnergy++;
    Energy = MaxEnergy;

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

void Player::PromptAuxCardChoice(AuxiliaryCardType type, const std::vector<Card*>& options) {
    std::cout << "--PICK FROM OPTIONS--" << "\n";
    for (size_t i = 0; i < options.size(); ++i) {
        std::cout << i + 1 << ". " << options[i]->Name << "\n";
    }

    int choice = 0;
    std::cin >> choice;
    choice = std::clamp(choice, 1, (int)options.size());

    AddChosenAuxCard(options[choice - 1]);
}
