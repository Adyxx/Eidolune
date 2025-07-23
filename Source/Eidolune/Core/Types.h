
#pragma once

#include <string>

enum class EquipmentSlot {
    HEAD,
    BODY,
    HANDS,
    LEGS
};

enum class TargetHint {
    NONE,            
    CARD,           
    PLAYER,       
    CARD_OR_PLAYER,  
    SELF,           
    UNKNOWN         
};

typedef std::string CardId;
typedef std::string EffectId;