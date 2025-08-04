

#include "GamblerClass.h"
#include <iostream>

GamblerClass::GamblerClass() {
    Name = "GamblerClass";
    Description = "";
}

void GamblerClass::OnTurnStart(Player* player) {
    std::cout << "[Gambler Turn Start Triggered]\n";
}