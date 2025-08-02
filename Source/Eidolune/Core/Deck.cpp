#include "Deck.h"
#include "Card.h"
#include "Character.h"
#include "DeckCard.h"
#include "User.h"
#include "EffectRegistry.h"
#include "CharacterRegistry.h"
#include "UserRegistry.h"
#include "CardRegistry.h"
#include <memory>

Deck::Deck(std::shared_ptr<User> user,
         const std::string& name,
         std::shared_ptr<Character> mainChar,
         std::shared_ptr<Character> partnerChar,
         const std::string& desc)
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


std::shared_ptr<Deck> Deck::FromJson(const nlohmann::json& j) {
    int ownerId = j.at("owner").get<int>();
    int mainCharId = j.at("main_character").get<int>();

    auto owner = UserRegistry::Instance().Get(ownerId);
    auto mainChar = CharacterRegistry::Instance().Get(mainCharId);
    if (!owner || !mainChar) return nullptr;

    std::shared_ptr<Character> partner = nullptr;
    if (j.contains("partner_character") && !j["partner_character"].is_null()) {
        partner = CharacterRegistry::Instance().Get(j["partner_character"].get<int>());
    }

    auto deck = std::make_shared<Deck>(
        owner,
        j.at("name").get<std::string>(),
        mainChar,
        partner,
        j.value("description", "")
    );

    if (j.contains("deck_cards") && j["deck_cards"].is_array()) {
        for (const auto& dc : j["deck_cards"]) {
            if (!dc.contains("card")) continue;
            auto card = CardRegistry::Instance().Get(dc["card"].get<int>());
            if (!card) continue;

            auto deckCard = std::make_shared<DeckCard>(deck, card, dc.value("quantity", 1));
            deck->DeckCards.push_back(deckCard);
        }
    }

    return deck;
}

nlohmann::json Deck::ToJson() const {
    nlohmann::json j;
    j["owner"] = Owner ? Owner->Id : -1;
    j["name"] = Name;
    j["main_character"] = MainCharacter ? MainCharacter->ID : -1;
    
    if (PartnerCharacter) {
        j["partner_character"] = PartnerCharacter->ID;
    } else {
        j["partner_character"] = nullptr;
    }

    j["description"] = Description;

    j["deck_cards"] = nlohmann::json::array();
    for (const auto& dc : DeckCards) {
        if (!dc || !dc->CardRef) continue;
        j["deck_cards"].push_back({
            {"card", dc->CardRef->ID},
            {"quantity", dc->Quantity}
        });
    }

    return j;
}