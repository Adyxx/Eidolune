#include "GameCard.h"
#include "Player.h"
#include <iostream>

static int GlobalGameCardId = 0;

GameCard::GameCard(std::shared_ptr<Card> model)
    : Model(model) {
        Id = GlobalGameCardId++;
         std::cout << "Created GameCard with ID: " << Id << " and model name: " << model->Name << "\n";
    }

std::string GameCard::GetName() const {
    return Model ? Model->Name : "";
}

int GameCard::GetCost() const {
    return Model ? Model->Cost : 0;
}

std::optional<int> GameCard::GetPower() const {
    return Model ? Model->Power : std::nullopt;
}

std::optional<int> GameCard::GetHealth() const {
    return Model ? Model->Health : std::nullopt;
}

int GameCard::CurrentHealth() const {
    if (!Model || !Model->Health.has_value()) return 0;
    return Model->Health.value() - DamageTaken;
}

std::string GameCard::ToString() const {
    std::string output = "<GameCard ";
    output += GetName();
    output += " (Cost: " + std::to_string(GetCost());
    if (GetPower()) {
        output += ", Power: " + std::to_string(GetPower().value());
    }
    if (GetHealth()) {
        output += ", Health: " + std::to_string(CurrentHealth());
    }
    output += ")>";
    return output;
}
