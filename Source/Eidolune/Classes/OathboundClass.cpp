

#include "OathboundClass.h"
#include <iostream>
#include "Card.h"
#include "CardRegistry.h"
#include "CardUtils.h"

OathboundClass::OathboundClass() {
    Name = "OathboundClass";
    Description = "";
}

void OathboundClass::OnTurnStart(Player* player) {
    int turn = player->GetTurnNumber();
    if (turn == 1 || turn == 4 || turn == 7) {
        auto allOaths = CardRegistry::GetCardsByAuxType(AuxiliaryCardType::OATH);

        std::vector<Card*> available;
        for (Card* card : allOaths) {
            if (!player->HasChosenAuxCard(card)) {
                available.push_back(card);
            }
        }

        auto options = CardUtils::SelectRandomOptions(available, 2);
        player->PromptAuxCardChoice(AuxiliaryCardType::OATH, options);
    }
}
