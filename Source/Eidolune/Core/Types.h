
#pragma once

#include <string>

enum class Rarity { COMMON, RARE, EPIC, LEGENDARY };
enum class CardType { SPELL, UNIT, EQUIPMENT, ASSET, EVENT, TRAP, RITUAL, OTHER };
enum class RelationshipType { PARTNER, RIVAL, FRIENDLY, LOVE, RESPECT };
enum class EquipmentSlot { HEAD, BODY, HANDS, LEGS };
enum class TargetHint { NONE, CARD, PLAYER, CARD_OR_PLAYER, SELF, UNKNOWN, ANY };
enum class TargetSpec { ENEMY_BOARD, ENEMY_HERO, ENEMY_BOARD_OR_HERO, FRIENDLY_BOARD, RANDOM_ENEMY, RANDOM_ALLY, FRIENDLY_HERO, SELF, ANY, UNKNOWN };
enum class ListeningZone { BOARD, HAND, DECK, GRAVEYARD, OATHZONE, ANY };
enum class CardZone { DECK, HAND, BOARD, GRAVEYARD, CONSUMABLE_BAG, OATHZONE, EXILE, UNKNOWN };
enum class Gender { MALE, FEMALE, NONBINARY, UNKNOWN };
enum class CharacterClassType { SPECIALIST, SPELLWEAVER, BLOODBOUND, SHADE, MACHINIST, SOULFORGED, GAMBLER, OATHBOUND, UNKNOWN };

enum class AuxiliaryCardType {
    NONE,
    REAGENT,
    OATH,
    CONSUMABLE,
    FRAGMENT,
    RELIC
};

enum class TriggerScope {
    SELF,               // Only if this card triggered the event
    OTHER_FRIENDLY,     // Another card from same owner
    ANY_FRIENDLY,
    GLOBAL,
    OTHER_ENEMY,        // Another card from opposing player
    ANY_OTHER,          // Any card, as long as it's not self
    ANY,                // All cards can respond
};

// LATER....
enum class CardState { NORMAL, DISABLED, EXHAUSTED, FROZEN, SILENCED, STEALTH, INVISIBLE, ATTACKING, DEFENDING };

