
#pragma once
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>

#include "../Core/Card.h"

#include <nlohmann/json.hpp>
#include <fstream>  
#include <iostream>

struct DeckInfo {
    std::string Name;
    std::vector<std::shared_ptr<Card>> Cards;
};

struct UserInfo {
    std::string Username;
    std::vector<DeckInfo> Decks;
};

class UserLoader {
public:
    static std::vector<UserInfo> LoadUsers(const std::string& userDir, const std::unordered_map<std::string, std::shared_ptr<Card>>& allCards) {
        std::vector<UserInfo> users;

        // For each user directory or file, load their decks
        // This is a simplified example assuming one user JSON file:
        std::ifstream file(userDir + "/user_1_decks.json");
        if (!file.is_open()) throw std::runtime_error("Could not open user decks file");

        nlohmann::json j;
        file >> j;

        UserInfo user;
        user.Username = "User1";

        for (auto& deckJson : j) {
            DeckInfo deck;
            deck.Name = deckJson["name"];

            for (const auto& cardId : deckJson["cards"]) {
                auto it = allCards.find(cardId.get<std::string>());
                if (it != allCards.end()) {
                    deck.Cards.push_back(it->second);
                } else {
                    std::cerr << "⚠️ Unknown card in deck: " << cardId << "\n";
                }
            }

            user.Decks.push_back(deck);
        }

        users.push_back(user);
        return users;
    }
};
