

// zone changes, movement on board, exile, destroy

#include "Target.h"
#include <optional>
#include "GameCard.h"
#include "EffectHelpers.h"
#include "EffectRegistry.h"

void ExileSelf(void* source, Target, std::optional<int>) {
    auto* card = static_cast<GameCard*>(source);
    if (!card || !card->Owner) return;

    auto& player = card->Owner;

    bool moved = EffectHelpers::MoveCardToZone(card, player, player->Hand, player->ExileZone, "Hand", "Exile")
              || EffectHelpers::MoveCardToZone(card, player, player->Board, player->ExileZone, "Board", "Exile")
              || EffectHelpers::MoveCardToZone(card, player, player->Graveyard, player->ExileZone, "Graveyard", "Exile")
              || EffectHelpers::MoveCardToZone(card, player, player->DrawPile, player->ExileZone, "DrawPile", "Exile")
              || EffectHelpers::MoveCardToZone(card, player, player->OathZone, player->ExileZone, "OathZone", "Exile");

    if (!moved) {
        std::cout << "⚠️ Could not find card in any zone.\n";
    }

    card->Zone = CardZone::EXILE;
}

void DestroyTarget(void* source, Target target, std::optional<int>) {
    if (target.type != Target::Type::CARD) return;
    auto* card = static_cast<GameCard*>(target.ptr);
    if (!card || !card->Owner) return;

    if (card->Owner->RemoveCardFromBoard(card)) {
        std::cout << "💀 Destroyed '" << card->GetName() << "' and sent to graveyard.\n";
    } else {
        std::cout << "⚠️ DestroyTarget: Card not found on board.\n";
    }
}

void ReturnToHand(void* source, Target target, std::optional<int>) {
    if (target.type != Target::Type::CARD) return;
    auto* card = static_cast<GameCard*>(target.ptr);
    if (!card || !card->Owner) return;

    Player* owner = card->Owner;

    // Remove from board
    for (int r = 0; r < Player::BoardHeight; ++r) {
        for (int c = 0; c < Player::BoardWidth; ++c) {
            if (owner->GridBoard[r][c] && owner->GridBoard[r][c].get() == card) {
                owner->Hand.push_back(owner->GridBoard[r][c]);
                owner->GridBoard[r][c] = nullptr;
                card->Zone = CardZone::HAND;
                std::cout << "↩️ Returned '" << card->GetName() << "' to hand.\n";
                return;
            }
        }
    }

    std::cout << "⚠️ ReturnToHand: Card not found on board.\n";
}

void ReviveFromGraveyard(void* source, Target target, std::optional<int>) {
    auto* player = static_cast<Player*>(target.ptr);
    if (!player) return;

    if (player->Graveyard.empty()) {
        std::cout << "⚠️ No cards in graveyard to revive.\n";
        return;
    }

    // Let player choose from graveyard
    std::vector<std::string> names;
    for (auto& c : player->Graveyard) names.push_back(c->GetName());

    int choiceIndex = player->PromptChooseOption(names);
    if (choiceIndex < 0 || choiceIndex >= (int)player->Graveyard.size()) return;

    auto chosenCard = player->Graveyard[choiceIndex];
    auto freePos = player->GetRandomFreeBoardSlot();

    if (!freePos) {
        std::cout << "🚫 No free board slot to revive card.\n";
        return;
    }

    Position pos = *freePos;
    player->GridBoard[pos.row][pos.col] = chosenCard;
    chosenCard->Zone = CardZone::BOARD;

    // Remove from graveyard
    player->Graveyard.erase(player->Graveyard.begin() + choiceIndex);

    std::cout << "❤️‍🩹 Revived '" << chosenCard->GetName() << "' at ("
              << pos.row << "," << pos.col << ").\n";
}



void RegisterEffectFunctions_CardMovement() {
    std::cout << "📦 Registering CardMovement effects...\n";

    EffectRegistry::Instance().Register("destroy_target", DestroyTarget);
    EffectRegistry::Instance().Register("return_to_hand", ReturnToHand);
    EffectRegistry::Instance().Register("revive_from_graveyard", ReviveFromGraveyard);
    EffectRegistry::Instance().Register("exile_self", ExileSelf);


}



