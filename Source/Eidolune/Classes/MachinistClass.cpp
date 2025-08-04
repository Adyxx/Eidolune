

#include "MachinistClass.h"
#include <iostream>

MachinistClass::MachinistClass() {
    Name = "MachinistClass";
    Description = "";
}

void MachinistClass::OnTurnStart(Player* player) {
    std::cout << "[Machinist Turn Start Triggered]\n";
}