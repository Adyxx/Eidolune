

#include "Types.h"
#include "GearEffectBinding.h"
#include <vector>

struct Equipment {
    std::string id;
    std::string name;
    EquipmentSlot slot;
    std::string setId;
    std::vector<GearEffectBinding> effects;
};