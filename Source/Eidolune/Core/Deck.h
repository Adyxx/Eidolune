
#pragma once
#include <string>
#include <vector>
#include <memory>

class User;
class Character;
class DeckCard;

class Deck {
public:
    std::shared_ptr<User> Owner;
    std::string Name;
    
    std::shared_ptr<Character> MainCharacter;
    std::shared_ptr<Character> PartnerCharacter;

    std::string Description;
    std::vector<std::shared_ptr<DeckCard>> DeckCards;

    Deck(std::shared_ptr<User> user,
         const std::string& name,
         std::shared_ptr<Character> mainChar,
         std::shared_ptr<Character> partnerChar = nullptr,
         const std::string& desc = "");

    int TotalCardCount() const;
    bool IsPlayable() const;
    std::vector<std::string> GetDeckIssues() const;

    std::string ToString() const;

};
