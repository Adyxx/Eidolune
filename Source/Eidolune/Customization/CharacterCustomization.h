#pragma once

class Character;
class Player;

#include <memory>

class CharacterCustomization {
public:
    static void Run();

    static void EquipSpecialistConsumable(std::shared_ptr<Character> character);
};
