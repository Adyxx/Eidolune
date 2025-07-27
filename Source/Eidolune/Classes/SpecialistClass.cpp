
#include "SpecialistClass.h"
#include <iostream>

SpecialistClass::SpecialistClass() {
    Name = "Specialist";
    Description = "Bring bag of consumables to the game. Combine ingredients and interract with the bag.";
}

void SpecialistClass::OnTurnStart(GameState* state, Player* player) {
    std::cout << "[Specialist Turn Start Triggered]\n";
}