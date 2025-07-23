

#include <map>
#include <string>
#include <vector>
#include "Effect.h"

struct EquipmentSet {
    std::string id;
    std::string name;
    std::map<int, std::vector<Effect>> bonuses;
};
