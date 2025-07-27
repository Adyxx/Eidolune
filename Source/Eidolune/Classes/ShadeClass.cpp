

#include "ShadeClass.h"
#include <iostream>

ShadeClass::ShadeClass() {
    Name = "ShadeClass";
    Description = "";
}

void ShadeClass::OnTurnStart(GameState* state, Player* player) {
    std::cout << "[Shade Turn Start Triggered]\n";
}