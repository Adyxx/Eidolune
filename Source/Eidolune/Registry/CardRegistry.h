#pragma once
#include <unordered_map>
#include <memory>
#include <string>
#include "../Core/Card.h"

#pragma once
#include <unordered_map>
#include <memory>
#include "../Core/Card.h"

class CardRegistry {
public:
    static CardRegistry& Instance();

    void Register(int id, std::shared_ptr<Card> card);
    std::shared_ptr<Card> Get(int id) const; 

    const std::unordered_map<int, std::shared_ptr<Card>>& GetAll() const {
        return Cards;
    }

    static std::vector<std::shared_ptr<Card>> GetCardsByAuxType(AuxiliaryCardType type);
    
private:
    std::unordered_map<int, std::shared_ptr<Card>> Cards;

    CardRegistry() = default;
    CardRegistry(const CardRegistry&) = delete;
    CardRegistry& operator=(const CardRegistry&) = delete;
};
