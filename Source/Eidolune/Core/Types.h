
#pragma once

#include <string>

enum class Rarity { COMMON, RARE, EPIC, LEGENDARY };
enum class CardType { SPELL, UNIT, ASSET, EVENT,  RITE, TRAP, RITUAL, OTHER, UNKNOWN };
enum class RelationshipType { PARTNER, RIVAL, FRIENDLY, LOVE, RESPECT };
enum class EquipmentSlot { HEAD, BODY, HANDS, LEGS };
enum class TargetHint { NONE, CARD, PLAYER, CARD_OR_PLAYER, SELF, UNKNOWN, ANY };
enum class TargetSpec { ENEMY_BOARD, ENEMY_HERO, ENEMY_BOARD_OR_HERO, FRIENDLY_BOARD, RANDOM_ENEMY, RANDOM_ALLY, FRIENDLY_HERO, SELF, ANY, SELF_EQUIPPED_UNIT, UNKNOWN };
enum class ListeningZone { BOARD, HAND, DECK, GRAVEYARD, OATHZONE, ANY };
enum class CardZone { DECK, HAND, BOARD, GRAVEYARD, CONSUMABLE_BAG, OATHZONE, EXILE, UNKNOWN };
enum class Gender { MALE, FEMALE, NONBINARY, UNKNOWN };
enum class CharacterClassType { SPECIALIST, SPELLWEAVER, BLOODBOUND, SHADE, MACHINIST, SOULFORGED, GAMBLER, OATHBOUND, UNKNOWN };

enum class TargetingRule {
    MANUAL,     // Player chooses one from the set
    AOE,        // All valid targets
    RANDOM,     // Random one valid target
    WEAKEST,    // Lowest HP
    STRONGEST,  // Highest Attack or HP
    NONE        // For effects that don’t actually target
};

enum class AuxiliaryCardType {
    NONE,
    REAGENT,
    OATH,
    CONSUMABLE,
    FRAGMENT,
    RELIC,
    TEMPLATE
};

enum class TriggerScope {
    SELF,               // Only if this card triggered the event
    OTHER_FRIENDLY,     // Another card from same owner
    ANY_FRIENDLY,
    GLOBAL,
    OTHER_ENEMY,        // Another card from opposing player
    ANY_OTHER,          // Any card, as long as it's not self
    ANY,                // All cards can respond
    EQUIPPED_UNIT
};


enum class CardStatus {
    NONE            = 0,
    HEXPROOF        = 1 << 0,
    SILENCED        = 1 << 1,
    SHROUDED        = 1 << 2,
    INVULNERABLE    = 1 << 3,
    MERGED          = 1 << 4
    // add more as needed
};
// LATER....
//enum class CardStatus { NONE, DISABLED, EXHAUSTED, FROZEN, SILENCED, STEALTH, INVISIBLE };

enum class DynamicValueType {
    STATIC_NUMBER,
    YOUR_CARDS_IN_HAND,
    ENEMY_CARDS_IN_HAND,
    FRIENDLY_BOARD_COUNT,
    ENEMY_BOARD_COUNT,
    FRIENDLY_GRAVEYARD_COUNT,
    ENEMY_GRAVEYARD_COUNT,
    TURN_NUMBER,
    OWNER_HEALTH,
    OPPONENT_HEALTH,
    UNKNOWN
};
