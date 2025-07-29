#pragma once
#include <unordered_map>
#include <memory>
#include <string>
#include "../Core/Deck.h"

class DeckRegistry {
public:
    static DeckRegistry& Instance();

    void Register(int id, std::shared_ptr<Deck> deck);
    std::shared_ptr<Deck> Get(int id) const;

    const std::unordered_map<int, std::shared_ptr<Deck>>& GetAll() const {
        return Decks;
    }

private:
    std::unordered_map<int, std::shared_ptr<Deck>> Decks;  
    DeckRegistry() = default;
    DeckRegistry(const DeckRegistry&) = delete;
    DeckRegistry& operator=(const DeckRegistry&) = delete;
};