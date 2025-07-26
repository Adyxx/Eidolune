#pragma once
#include <string>
#include <memory>
#include <vector>
#include "Character.h"
#include "TalentNode.h"

class TalentTree {
public:
    std::shared_ptr<Character> CharacterRef;
    std::vector<TalentNode> Nodes;
};
