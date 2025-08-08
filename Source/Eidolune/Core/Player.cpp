#include "Player.h"
#include <iostream>
#include "EffectDefinitions.h"
#include "Target.h"
#include "CardUtils.h"
#include "Card.h"

Player::Player(std::shared_ptr<Deck> deck, int index)
    : DeckRef(deck), PlayerIndex(index) {}

std::string Player::GetName() const {
    return "Player " + std::to_string(PlayerIndex + 1);
}

void Player::StartTurn() {
    TurnCount++;
    CardsPlayedThisTurn = 0;
    DealtDamageToEnemyHeroThisTurn = false;
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

    for (int row = 0; row < BoardHeight; ++row) {
        for (int col = 0; col < BoardWidth; ++col) {
            auto& card = GridBoard[row][col];
            if (card) {
                card->Tapped = false;
                card->SummoningSickness = false;
            }
        }
    }
}

void Player::PromptAuxCardChoice(AuxiliaryCardType type, const std::vector<std::shared_ptr<Card>>& options) {
    std::cout << "--PICK FROM OPTIONS--" << "\n";
    for (size_t i = 0; i < options.size(); ++i) {
        std::cout << i + 1 << ". " << options[i]->Name << "\n";
    }

    int choice = 0;
    std::cin >> choice;
    choice = std::clamp(choice, 1, static_cast<int>(options.size()));

    AddChosenAuxCard(options[choice - 1]);
}


bool Player::HasChosenAuxCard(const std::shared_ptr<Card>& card) const {
    auto it = ChosenAuxiliaryCards.find(card->AuxilaryType);
    if (it == ChosenAuxiliaryCards.end()) return false;

    return std::find(it->second.begin(), it->second.end(), card) != it->second.end();
}

void Player::AddChosenAuxCard(std::shared_ptr<Card> card) {
    ChosenAuxiliaryCards[card->AuxilaryType].push_back(card);

    if (card->AuxilaryType == AuxiliaryCardType::OATH) {
        auto gameCard = CardUtils::RegisterCardMidGame(card, this, CardZone::OATHZONE, GetTriggerObserver());
        //OathZone.push_back(gameCard);
        //std::cout << "🛡️ Oath card '" << card->Name << "' added to OathZone.\n";
    }
}

int Player::PromptChooseOption(const std::vector<std::string>& options) {
    // std::cout << "Player " << player->UserName << ", choose one:\n";
    std::cout << "--PICK FROM OPTIONS--" << "\n";
    for (size_t i = 0; i < options.size(); ++i) {
        std::cout << "  [" << i << "] " << options[i] << "\n";
    }

    int choice = -1;
    std::cin >> choice; // Replace with proper UI/input method
    return choice;
}

    std::vector<std::shared_ptr<GameCard>> Player::GetAllBoardCards() {
        std::vector<std::shared_ptr<GameCard>> cards;

        for (int row = 0; row < Player::BoardHeight; ++row) {
            for (int col = 0; col < Player::BoardWidth; ++col) {
                if (GridBoard[row][col]) {
                    cards.push_back(GridBoard[row][col]);
                }
            }
        }

        return cards;
    }

    bool Player::RemoveCardFromBoard(GameCard* card) {
    for (int row = 0; row < Player::BoardHeight; ++row) {
        for (int col = 0; col < Player::BoardWidth; ++col) {
            if (GridBoard[row][col] && GridBoard[row][col].get() == card) {

                Graveyard.push_back(GridBoard[row][col]);
                GridBoard[row][col] = nullptr;

                return true;
            }
        }
    }

    return false;
}

void Player::RemoveAllAurasFromSource(int sourceId) {
    for (int row = 0; row < Player::BoardHeight; ++row) {
        for (int col = 0; col < Player::BoardWidth; ++col) {
            if (GridBoard[row][col] && GridBoard[row][col]->ActiveAuras.count(sourceId)) {
                GridBoard[row][col]->RemoveAura(sourceId);
            }
        }
    }
}

    std::shared_ptr<GameCard> Player::GetCardAt(const Position& pos) const {
        if (!pos.IsValid()) return nullptr;
        return GridBoard[pos.row][pos.col];
    }

    bool Player::IsOccupied(const Position& pos) const {
        return GetCardAt(pos) != nullptr;
    }
    