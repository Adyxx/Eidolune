
#pragma once
#include "Types.h"
#include "GearEffectBinding.h"
#include <vector>

class Gear {
    std::string id;
    std::string name;
    //EquipmentSlot slot;
    std::string setId;
    std::vector<GearEffectBinding> effects;

    std::string ToString() const;
};