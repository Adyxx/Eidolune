#include "Deck.h"
#include "Card.h"
#include "Character.h"
#include "DeckCard.h"
#include "User.h"
#include "EffectRegistry.h"
#include "DeckRestrictionRegistry.h"


Deck::Deck(std::shared_ptr<User> user,
         const std::string& name,
         std::shared_ptr<Character> mainChar,
         std::shared_ptr<Character> partnerChar = nullptr,
         const std::string& desc = "")
        : Owner(user), Name(name), MainCharacter(mainChar),
          PartnerCharacter(partnerChar), Description(desc) {}



int Deck::TotalCardCount() const {
    int total = 0;
    for (const auto& dc : DeckCards)
        total += dc->Quantity;
    return total;
}

bool Deck::IsPlayable() const {
    return GetDeckIssues().empty();
}

std::vector<std::string> Deck::GetDeckIssues() const {
    std::vector<std::string> issues;
    return issues;
}

std::string Deck::ToString() const {
    return Name + " (" + Owner->ToString() + ")";
}
