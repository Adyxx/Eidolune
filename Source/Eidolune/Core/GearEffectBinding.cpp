
#pragma once

#include <string>
#include "Condition.h"

struct GearEffectBinding {
    std::string triggerId; 
    std::string effectId; 
    Condition* condition;
};
