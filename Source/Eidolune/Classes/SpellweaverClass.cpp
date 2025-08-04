
#include "SpellweaverClass.h"
#include <iostream>

SpellweaverClass::SpellweaverClass() {
    Name = "SpellweaverClass";
    Description = "";
}

void SpellweaverClass::OnTurnStart(Player* player) {
    std::cout << "[Spellweaver Turn Start Triggered]\n";
}

void SpellweaverClass::UseClassAbility(Character* character, Player* player, std::shared_ptr<TriggerObserver> observer) {
    
}