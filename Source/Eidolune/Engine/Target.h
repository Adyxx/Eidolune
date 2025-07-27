
#pragma once

#include "Player.h"
#include "GameCard.h"
#include <iostream>

struct Target {
    enum class Type { NONE, CARD, PLAYER, UNKNOWN } type = Type::NONE;
    void* ptr = nullptr;

    static Target FromCard(GameCard* card) {
        return Target{Type::CARD, card};
    }
    static Target FromPlayer(Player* player) {
        return Target{Type::PLAYER, player};
    }

};
