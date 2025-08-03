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


std::shared_ptr<Deck> Deck::FromJson(const nlohmann::json& j, std::shared_ptr<User> owner) {
    if (!j.contains("name") || !j.contains("main_character")) {
        std::cerr << "❌ Deck JSON missing required fields.\n";
        return nullptr;
    }

    if (!owner) {
        std::cerr << "❌ Cannot create deck: null owner passed in.\n";
        return nullptr;
    }

    int mainCharId = j["main_character"].get<int>();

    std::shared_ptr<Character> mainChar = nullptr;
    if (mainCharId != -1) {
        mainChar = CharacterRegistry::Instance().Get(mainCharId);
        if (!mainChar) {
            std::cerr << "⚠️ Main character (ID " << mainCharId << ") not found.\n";
        }
    }

    std::shared_ptr<Character> partner = nullptr;
    if (j.contains("partner_character") && !j["partner_character"].is_null()) {
        int partnerId = j["partner_character"].get<int>();
        partner = CharacterRegistry::Instance().Get(partnerId);
        if (!partner) {
            std::cerr << "⚠️ Partner character (ID " << partnerId << ") not found.\n";
        }
    }

    auto deck = std::make_shared<Deck>(
        owner,
        j.value("name", "Unnamed Deck"),
        mainChar,
        partner,
        j.value("description", "")
    );
    
    deck->ID = j.value("id", -1);

    // Load cards
    if (j.contains("cards") && j["cards"].is_array()) {
        for (const auto& dc : j["cards"]) {
            if (!dc.contains("card_id")) continue;

            int cardId = dc["card_id"].get<int>();
            auto card = CardRegistry::Instance().Get(cardId);
            if (!card) {
                std::cerr << "⚠️ Deck card ID " << cardId << " not found.\n";
                continue;
            }

            int qty = dc.value("quantity", 1);
            auto deckCard = std::make_shared<DeckCard>(deck, card, qty);
            deck->DeckCards.push_back(deckCard);
        }
    }

    return deck;
}


nlohmann::json Deck::ToJson() const {
    nlohmann::json j;

    j["id"] = ID; 
    j["name"] = Name;
    j["main_character"] = MainCharacter ? MainCharacter->ID : -1;
    if (PartnerCharacter) {
        j["partner_character"] = PartnerCharacter->ID;
    } else {
        j["partner_character"] = nullptr;
    }

    j["cards"] = nlohmann::json::array();
    for (const auto& dc : DeckCards) {
        if (!dc || !dc->CardRef) continue;
        j["cards"].push_back({
            {"card_id", dc->CardRef->ID},
            {"quantity", dc->Quantity}
        });
    }

    return j;
}