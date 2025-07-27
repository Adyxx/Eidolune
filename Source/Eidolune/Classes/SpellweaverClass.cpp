
#include "SpellweaverClass.h"
#include <iostream>

SpellweaverClass::SpellweaverClass() {
    Name = "SpellweaverClass";
    Description = "";
}

void SpellweaverClass::OnTurnStart(GameState* state, Player* player) {
    std::cout << "[Spellweaver Turn Start Triggered]\n";
}