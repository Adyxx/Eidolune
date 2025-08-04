

#include "OathboundClass.h"
#include <iostream>

OathboundClass::OathboundClass() {
    Name = "OathboundClass";
    Description = "";
}

void OathboundClass::OnTurnStart(Player* player) {
    std::cout << "[Oathbound Turn Start Triggered]\n";
}