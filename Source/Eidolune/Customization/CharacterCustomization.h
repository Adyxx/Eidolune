#pragma once


#include "UserData.h"
#include "User.h"

class Character;
class Player;

#include <memory>

class CharacterCustomization {
public:
    static void Run(std::shared_ptr<User> user, UserData& userData);

    static void EquipSpecialistConsumable(std::shared_ptr<Character> character);
};
