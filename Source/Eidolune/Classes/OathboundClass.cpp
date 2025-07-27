

#include "OathboundClass.h"
#include <iostream>

OathboundClass::OathboundClass() {
    Name = "OathboundClass";
    Description = "";
}

void OathboundClass::OnTurnStart(GameState* state, Player* player) {
    std::cout << "[Oathbound Turn Start Triggered]\n";
}