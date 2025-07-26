#pragma once
#include <memory>
#include <vector>
#include "Character.h"
#include "TalentNode.h"

class CharacterProgress {
public:
    std::shared_ptr<Character> CharacterRef;
    int CurrentStars = 0;
    int CopiesOwned = 0;
    bool IsUnlocked = false;
    std::vector<std::shared_ptr<TalentNode>> UnlockedTalents;
};
