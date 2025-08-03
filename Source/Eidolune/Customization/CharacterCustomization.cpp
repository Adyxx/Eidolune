#include "CharacterCustomization.h"
#include "../Registry/CharacterRegistry.h"
#include "../Core/Character.h"
#include <iostream>
#include <vector>
#include <memory>
#include "../Classes/SpecialistClass.h"
#include "../Registry/CharacterRegistry.h"
#include "../Registry/CardRegistry.h"

#include "../Registry/CardRegistry.h"
#include "../Core/Player.h"

#include "UserCharacterData.h"
#include "UserDataUploader.h"


void CharacterCustomization::EquipSpecialistConsumable(std::shared_ptr<Character> character) {
    if (!character || !character->ClassLogic) {
        std::cerr << "❌ Character or class logic is missing.\n";
        return;
    }

    auto specialist = std::dynamic_pointer_cast<SpecialistClass>(character->ClassLogic);
    if (!specialist) {
        std::cerr << "❌ This character is not a Specialist.\n";
        return;
    }

    auto& bag = specialist->GetBag(character.get()); 

    while (true) {
        std::cout << "\n🎒 Consumable Slots:\n";
        for (int i = 0; i < bag.slots.size(); ++i) {
            const auto& slot = bag.slots[i];
            std::cout << i << ": ";
            if (slot.item && slot.item->SourceCard) {
                std::cout << "<" << slot.item->SourceCard->Name << " - Uses: " << slot.item->UsesRemaining << ">\n";
            } else {
                std::cout << "<empty>\n";
            }
        }
        std::cout << "3: Back\n";
        std::cout << "Choose a slot to modify: ";
        int slotChoice;
        std::cin >> slotChoice;
        std::cin.ignore();

        if (slotChoice == 3) break;
        if (slotChoice < 0 || slotChoice > 2) {
            std::cout << "❌ Invalid slot.\n";
            continue;
        }

        // Get all consumables from CardRegistry
        std::vector<std::shared_ptr<Card>> valid;
        const auto& allCards = CardRegistry::Instance().GetAll();
        for (const auto& [_, card] : allCards) {
            if (card && card->IsConsumable()) {
                valid.push_back(card);
            }
        }

        if (valid.empty()) {
            std::cout << "📦 No consumables available.\n";
            continue;
        }

        std::cout << "\nAvailable Consumables:\n";
        for (size_t i = 0; i < valid.size(); ++i) {
            std::cout << i << ": " << valid[i]->Name << " (Cost: " << valid[i]->Cost << ")\n";
        }
        std::cout << valid.size() << ": Unequip slot\n";
        std::cout << valid.size() + 1 << ": Cancel\n";
        std::cout << "Select a consumable: ";

        int itemChoice;
        std::cin >> itemChoice;
        std::cin.ignore();

        if (itemChoice == static_cast<int>(valid.size())) {
            specialist->EquipConsumable(character.get(), nullptr, slotChoice);
        } else if (itemChoice == static_cast<int>(valid.size()) + 1) {
            continue;
        } else if (itemChoice >= 0 && itemChoice < static_cast<int>(valid.size())) {
            specialist->EquipConsumable(character.get(), valid[itemChoice], slotChoice);
        } else {
            std::cout << "❌ Invalid choice.\n";
        }
    }
}


void CharacterCustomization::Run(std::shared_ptr<User> user, UserData& userData) {
    const auto& all = CharacterRegistry::Instance().GetAll();

    if (all.empty()) {
        std::cout << "❌ No characters loaded.\n";
        return;
    }

    std::vector<std::shared_ptr<Character>> characters;
    for (const auto& [_, character] : all) {
        characters.push_back(character);
    }

    while (true) {
        std::cout << "\n=== 🎨 Character Customization ===\n";
        for (size_t i = 0; i < characters.size(); ++i) {
            const auto& character = characters[i];
            bool owned = UserCharacterData::IsCharacterOwned(character->ID, userData.Characters);
            std::cout << i << ": " << character->Name;
            
            if (character->Class == CharacterClassType::SPECIALIST) {
                std::cout << " [Specialist]";
            }
            if (owned) {
                std::cout << " [OWNED]";
            }

            std::cout << "\n";
        }

        std::cout << characters.size() << ": Return to main menu\n";
        std::cout << "Select a character: ";

        int choice;
        std::cin >> choice;
        std::cin.ignore();

        if (choice < 0 || choice > characters.size()) {
            std::cout << "❓ Invalid input.\n";
            continue;
        }
        if (choice == characters.size()) {
            break;
        }

        auto& selected = characters[choice];
        std::cout << "🔧 Customizing " << selected->Name << "\n";

        while (true) {
            std::cout << "\n🛠️  Customization Options:\n";
            std::cout << "0: Back\n";
            std::cout << "1: Equip Gear (todo)\n";
            std::cout << "2: Upgrade Skill Tree (todo)\n";
            std::cout << "3: Change Level (todo)\n";

            if (selected->Class == CharacterClassType::SPECIALIST) {
                std::cout << "4: Equip Consumable\n";
            }

            std::cout << "Choose an option: ";
            int opt;
            std::cin >> opt;
            std::cin.ignore();

            if (opt == 0) break;

            if (selected->Class == CharacterClassType::SPECIALIST && opt == 4) {
                EquipSpecialistConsumable(selected);
            } else {
                std::cout << "⚠️  Option not implemented yet.\n";
            }

            // Sync changes to userData and push to server
            UserCharacterData updated = UserCharacterData::FromCharacter(selected.get());
            userData.SyncCharacter(updated);
            UserDataUploader::UploadUserData(user, userData);
            std::cout << "✅ Changes saved.\n";
        }
    }
}
