

#include "BloodboundClass.h"
#include <iostream>

BloodboundClass::BloodboundClass() {
    Name = "BloodboundClass";
    Description = "";
}

void BloodboundClass::OnTurnStart(GameState* state, Player* player) {
    std::cout << "[Bloodbound Turn Start Triggered]\n";
}