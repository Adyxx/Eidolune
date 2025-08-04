

#include "SoulforgedClass.h"
#include <iostream>

SoulforgedClass::SoulforgedClass() {
    Name = "SoulforgedClass";
    Description = "";
}

void SoulforgedClass::OnTurnStart(Player* player) {
    std::cout << "[SoulforgedClass Turn Start Triggered]\n";
}