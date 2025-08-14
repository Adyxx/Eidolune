#pragma once
#include <string>
#include <vector>
#include <functional>
#include "Player.h"

struct BloodboundNode {
    int Id;
    std::string Name;
    std::string Description;
    int EchoRequirement;
    std::function<void(Player*)> ApplyEffect;

    std::vector<int> NextNodes;
};
