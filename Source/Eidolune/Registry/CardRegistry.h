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

private:
    std::unordered_map<int, std::shared_ptr<Card>> Cards;

    CardRegistry() = default;
    CardRegistry(const CardRegistry&) = delete;
    CardRegistry& operator=(const CardRegistry&) = delete;
};
