
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

#include "../Registry/DeckRegistry.h"
#include "../Core/Deck.h"
#include "../Core/Character.h"
#include "../Core/DeckCard.h"
#include "../Registry/CharacterRegistry.h"
#include "../Registry/CardRegistry.h"
#include "../Core/Card.h"
#include "DeckValidator.h"
#include "DeckBuilder.h"
#include "UserDataUploader.h"

// Forward declarations for helper functions
namespace {
    int PromptInt(int min, int max, const std::string& prompt = "Choose option: ");
    void ListUserDecks(const std::vector<std::shared_ptr<Deck>>& decks);
    std::shared_ptr<Deck> CreateNewDeck(std::shared_ptr<User> user);
    void EditDeck(std::shared_ptr<Deck> deck);
    void RenameDeck(std::shared_ptr<Deck> deck);
    void ChangeDescription(std::shared_ptr<Deck> deck);
    void AssignMainCharacter(std::shared_ptr<Deck> deck);
    void ChangeCards(std::shared_ptr<Deck> deck);
    void ShowCardsInDeck(const std::shared_ptr<Deck>& deck);
    bool Confirm(const std::string& question);
}

void DeckBuilder::Run(std::shared_ptr<User> currentUser, UserData& currentUserData) {
    while (true) {
        auto allDecksMap = DeckRegistry::Instance().GetAll();
        std::vector<std::shared_ptr<Deck>> userDecks;
        for (const auto& [id, deck] : allDecksMap) {
            if (deck->Owner && deck->Owner->Id == currentUser->Id) {
                userDecks.push_back(deck);
            }
        }

        std::cout << "\n=== 📚 Deck Editor ===\n";
        ListUserDecks(userDecks);

        std::cout << userDecks.size() << ": --CREATE NEW DECK--\n";
        std::cout << userDecks.size() + 1 << ": Back\n";

        int choice = PromptInt(0, (int)userDecks.size() + 1);

        if (choice == (int)userDecks.size()) {
            // create new deck
            auto newDeck = CreateNewDeck(currentUser);
            if (newDeck) {
                EditDeck(newDeck);
                currentUserData.SyncDeck(newDeck);
                UserDataUploader::UploadUserData(currentUser, currentUserData);
            }
        } else if (choice == (int)userDecks.size() + 1) {
            // back to main menu
            break;
        } else {
            auto selectedDeck = userDecks[choice];
            while (true) {
                std::cout << "\nSelected deck: " << selectedDeck->Name << "\n";
                std::cout << "0: Edit\n";
                std::cout << "1: Delete\n";
                std::cout << "2: Back\n";

                int action = PromptInt(0, 2);

                if (action == 0) {
                    EditDeck(selectedDeck);
                    currentUserData.SyncDeck(selectedDeck);
                    UserDataUploader::UploadUserData(currentUser, currentUserData);
                } else if (action == 1) {
                    if (Confirm("Are you sure you want to delete this deck? (y/n): ")) {
                        DeckRegistry::Instance().Remove(selectedDeck->ID);
                        std::cout << "Deck deleted.\n";
                        break;
                    }
                } else {
                    break; // back to deck list
                }
            }
        }
    }
}

// --- Implementations of helper functions ---

namespace {

int PromptInt(int min, int max, const std::string& prompt) {
    int val;
    while (true) {
        std::cout << prompt;
        std::string line;
        std::getline(std::cin, line);
        try {
            val = std::stoi(line);
            if (val >= min && val <= max) {
                break;
            }
        } catch (...) {}
        std::cout << "Invalid input, try again.\n";
    }
    return val;
}

void ListUserDecks(const std::vector<std::shared_ptr<Deck>>& decks) {
    for (size_t i = 0; i < decks.size(); ++i) {
        std::cout << i << ": " << decks[i]->Name << "\n";
    }
}

std::shared_ptr<Deck> CreateNewDeck(std::shared_ptr<User> user) {
    static int nextId = 10000; // or fetch from somewhere reliable

    auto deck = std::make_shared<Deck>(
        user,             // owner
        "New Deck",       // name
        nullptr,          // mainChar
        nullptr,          // partnerChar
        ""                // description
    );
    deck->ID = nextId++;

    DeckRegistry::Instance().Register(deck->ID, deck);
    std::cout << "Created new deck with name: " << deck->Name << "\n";

    return deck;
}

void EditDeck(std::shared_ptr<Deck> deck) {
    while (true) {
        std::cout << "\nEditing deck: " << deck->Name << "\n";
        std::cout << "0: Rename\n";
        std::cout << "1: Change Description\n";
        std::cout << "2: Assign Main Character\n";
        std::cout << "3: Change Cards\n";
        std::cout << "4: Back\n";

        int choice = PromptInt(0, 4);

        switch (choice) {
            case 0: RenameDeck(deck); break;
            case 1: ChangeDescription(deck); break;
            case 2: AssignMainCharacter(deck); break;
            case 3: ChangeCards(deck); break;
            case 4: return;  // back to deck list
        }
    }
}

void RenameDeck(std::shared_ptr<Deck> deck) {
    std::cout << "Current Name: " << deck->Name << "\n";
    std::cout << "Enter new name: ";
    std::string newName;
    std::getline(std::cin, newName);
    if (!newName.empty()) {
        deck->Name = newName;
        std::cout << "Name updated.\n";
    } else {
        std::cout << "Name unchanged.\n";
    }
}

void ChangeDescription(std::shared_ptr<Deck> deck) {
    std::cout << "Current Description: " << deck->Description << "\n";
    std::cout << "Enter new description: ";
    std::string desc;
    std::getline(std::cin, desc);
    deck->Description = desc;
    std::cout << "Description updated.\n";
}

void AssignMainCharacter(std::shared_ptr<Deck> deck) {
    auto charactersMap = CharacterRegistry::Instance().GetAll();
    if (charactersMap.empty()) {
        std::cout << "No characters loaded.\n";
        return;
    }

    std::vector<std::shared_ptr<Character>> chars;
    for (const auto& [_, c] : charactersMap) {
        chars.push_back(c);
    }

    std::cout << "Choose a Main Character:\n";
    for (size_t i = 0; i < chars.size(); ++i) {
        std::cout << i << ": " << chars[i]->Name << "\n";
    }
    std::cout << chars.size() << ": None\n";

    int choice = PromptInt(0, (int)chars.size());

    if (choice == (int)chars.size()) {
        deck->MainCharacter = nullptr;
        std::cout << "Main character cleared.\n";
    } else {
        deck->MainCharacter = chars[choice];
        std::cout << "Main character set to " << deck->MainCharacter->Name << "\n";
    }
}

void ShowCardsInDeck(const std::shared_ptr<Deck>& deck) {
    if (deck->DeckCards.empty()) {
        std::cout << "<Empty deck>\n";
        return;
    }

    int index = 0;
    for (const auto& deckCard : deck->DeckCards) {
        if (!deckCard || !deckCard->CardRef) continue;
        std::cout << index++ << ": (" << deckCard->CardRef->ID << ") " << deckCard->Quantity << "x " << deckCard->CardRef->Name << "\n";
    }
}

void ChangeCards(std::shared_ptr<Deck> deck) {
    while (true) {
        ShowCardsInDeck(deck);
        int totalCards = 0;
        for (auto& dc : deck->DeckCards) {
            if (dc) totalCards += dc->Quantity;
        }
        std::cout << "Total cards: " << totalCards << "\n";

        std::cout << "\nOptions:\n";
        std::cout << "0: Remove Card by index\n";
        std::cout << "1: Add Card by ID and quantity\n";
        std::cout << "2: Back\n";

        int choice = PromptInt(0, 2);

        if (choice == 0) {
            std::cout << "Enter card index to remove: ";
            int idx;
            std::cin >> idx;
            std::cin.ignore();

            if (idx < 0 || idx >= (int)deck->DeckCards.size()) {
                std::cout << "Invalid index.\n";
                continue;
            }
            deck->DeckCards.erase(deck->DeckCards.begin() + idx);
            std::cout << "Card removed.\n";

        } else if (choice == 1) {
            std::cout << "Enter Card ID to add: ";
            int cardId;
            std::cin >> cardId;
            std::cout << "Enter quantity to add: ";
            int qty;
            std::cin >> qty;
            std::cin.ignore();

            if (qty <= 0) {
                std::cout << "Quantity must be positive.\n";
                continue;
            }

            // Find existing deckCard with this cardId
            auto it = std::find_if(deck->DeckCards.begin(), deck->DeckCards.end(), [cardId](const std::shared_ptr<DeckCard>& dc){
                return dc && dc->CardRef && dc->CardRef->ID == cardId;
            });

            if (it != deck->DeckCards.end()) {
                (*it)->Quantity += qty;
                std::cout << "Added " << qty << " more copies.\n";
            } else {
                // Look up CardRef in Card registry
                // For this example, assume CardRegistry similar to CharacterRegistry exists
                auto card = CardRegistry::Instance().Get(cardId);
                if (!card) {
                    std::cout << "Card ID not found.\n";
                    continue;
                }
                auto deckCard = std::make_shared<DeckCard>(deck, card, qty);
                deck->DeckCards.push_back(deckCard);
                std::cout << "Added new card to deck.\n";
            }

        } else {
            break; // back to edit menu
        }
    }
}

bool Confirm(const std::string& question) {
    std::cout << question;
    std::string ans;
    std::getline(std::cin, ans);
    return (!ans.empty() && (ans[0] == 'y' || ans[0] == 'Y'));
}

}
