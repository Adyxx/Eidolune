

#include "BloodboundClass.h"
#include <iostream>

BloodboundClass::BloodboundClass() {
    Name = "BloodboundClass";
    Description = "";
}

void BloodboundClass::OnTurnStart(Player* player) {
    std::cout << "[Bloodbound Turn Start Triggered]\n";
}

void BloodboundClass::ApplySetupBonus(Character* character) {
    
}