    #include "DeckValidator.h"
    #include "../Registry/DeckRestrictionRegistry.h"
    #include "../Core/CardEffectBinding.h"
    #include "../Core/DeckCard.h"
    #include "../Core/Card.h"
    #include "../Core/Character.h"

    #include <unordered_map>
    #include <unordered_set>
    #include <iostream>

    DeckValidationResult DeckValidator::Validate(std::shared_ptr<Deck> deck) {
        DeckValidationResult result;

        std::cout << "----" << deck->MainCharacter <<"\n";

        if (!deck) {
            result.IsPlayable = false;
            result.Issues.push_back("Deck object is null.");
            return result;
        }

        std::unordered_map<int, int> cardCounts;
        std::unordered_set<int> foundCharacterIds;

        int totalCardCount = 0;

        for (const auto& deckCard : deck->DeckCards) {
            if (!deckCard || !deckCard->CardRef) continue;

            auto card = deckCard->CardRef;
            int cardId = card->ID;
            int qty = deckCard->Quantity;

            totalCardCount += qty;
            cardCounts[cardId] += qty;

            int limit = card->IsCharacterCard ? 1 : 3;
            std::cout << totalCardCount << "  ...  " << "\n";
            for (const auto& binding : card->EffectBindings) {
                if (binding->GetTrigger() && binding->GetEffect()) {
                    if (binding->GetTrigger()->ScriptReference == "on_deck_build" &&
                        binding->GetEffect()->ScriptReference == "override_deck_limit") {

                        if (binding->GetValue().has_value()) {
                            limit = binding->GetValue().value();
                        } else {
                            result.Issues.push_back(card->Name + " has missing value for override_deck_limit.");
                        }
                    }
                }

            }

            if (cardCounts[cardId] > limit) {
                std::cout << "AxxxcxaA ....... " << cardCounts[cardId] << "       \n";
                result.Issues.push_back(card->Name + " exceeds allowed limit of " + std::to_string(limit) + " copies.");
                result.IsPlayable = false;
            }

            if (card->IsCharacterCard && card->CharacterRef) {
                int charId = card->CharacterRef->ID;
                if (foundCharacterIds.count(charId)) {
                    result.Issues.push_back("Multiple character cards for '" + card->CharacterRef->Name + "' in deck.");
                    result.IsPlayable = false;
                } else {
                    foundCharacterIds.insert(charId);
                }
            }

            if (card->IsCharacterExclusive && card->CharacterRef) {
                std::unordered_set<int> allowedCharIds = { deck->MainCharacter->ID };
                if (deck->PartnerCharacter) allowedCharIds.insert(deck->PartnerCharacter->ID);

                if (!allowedCharIds.count(card->CharacterRef->ID)) {
                    result.Issues.push_back(card->Name + " is exclusive to '" + card->CharacterRef->Name + "', who is not in this deck.");
                    result.IsPlayable = false;
                }
            }
        }

        if (totalCardCount < 40) {
            result.Issues.push_back("Deck has fewer than 40 cards.");
            result.IsPlayable = false;
        }

        if (deck->MainCharacter && !foundCharacterIds.count(deck->MainCharacter->ID)) {
            result.Issues.push_back("Missing character card for main character: " + deck->MainCharacter->Name);
            result.IsPlayable = false;
        }

        if (deck->PartnerCharacter && !foundCharacterIds.count(deck->PartnerCharacter->ID)) {
            result.Issues.push_back("Missing character card for partner: " + deck->PartnerCharacter->Name);
            result.IsPlayable = false;
        }

        for (const auto& character : { deck->MainCharacter, deck->PartnerCharacter }) {
            if (character && !character->DeckRestrictionRef.empty()) {
                auto func = DeckRestrictionRegistry::Get(character->DeckRestrictionRef);
                if (func) {
                    auto [ok, reason] = func(deck);
                    if (!ok) {
                        result.Issues.push_back("Deck restriction (" + character->Name + "): " + reason);
                        result.IsPlayable = false;
                    }
                }
            }
        }

        return result;
    }
