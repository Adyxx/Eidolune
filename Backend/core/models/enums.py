from django.db import models

class Rarity(models.TextChoices):
    COMMON = "COMMON"
    RARE = "RARE"
    EPIC = "EPIC"
    LEGENDARY = "LEGENDARY"

class CardType(models.TextChoices):
    SPELL = "SPELL"
    UNIT = "UNIT"
    EQUIPMENT = "EQUIPMENT"
    ASSET = "ASSET"
    EVENT = "EVENT"
    TRAP = "TRAP"
    RITUAL = "RITUAL"
    OTHER = "OTHER"

class TargetingRule(models.TextChoices):
    MANUAL = "MANUAL"
    AOE = "AOE"
    RANDOM = "RANDOM"
    WEAKEST = "WEAKEST"
    STRONGEST = "STRONGEST"
    NONE = "NONE"


class TargetSpec(models.TextChoices):
    ENEMY_BOARD = "ENEMY_BOARD"
    ENEMY_HERO = "ENEMY_HERO"
    ENEMY_BOARD_OR_HERO = "ENEMY_BOARD_OR_HERO"
    FRIENDLY_BOARD = "FRIENDLY_BOARD"
    FRIENDLY_HERO = "FRIENDLY_HERO"
    SELF = "SELF"
    ANY = "ANY"
    UNKNOWN = "UNKNOWN"

class ListeningZone(models.TextChoices):
    BOARD = "BOARD"
    HAND = "HAND"
    DECK = "DECK"
    GRAVEYARD = "GRAVEYARD"
    ANY = "ANY"
    OATHZONE = "OATHZONE"
    EXILE = "EXILE"
    CONSUMABLE_BAG = "CONSUMABLE_BAG"

class Gender(models.TextChoices):
    MALE = "MALE"
    FEMALE = "FEMALE"
    NONBINARY = "NONBINARY"
    UNKNOWN = "UNKNOWN"

class RelationshipType(models.TextChoices):
    PARTNER = "PARTNER"
    RIVAL = "RIVAL"
    FRIENDLY = "FRIENDLY"
    LOVE = "LOVE"
    RESPECT = "RESPECT"

class TriggerScope(models.TextChoices):
    SELF = "SELF"
    OTHER_FRIENDLY = "OTHER_FRIENDLY"
    ANY_FRIENDLY = "ANY_FRIENDLY"
    OTHER_ENEMY = "OTHER_ENEMY"
    ANY_OTHER = "ANY_OTHER"
    ANY = "ANY"
    GLOBAL = "GLOBAL"
    EQUIPPED_UNIT = "EQUIPPED_UNIT"

class CharacterClassType(models.TextChoices):
     SPECIALIST = "SPECIALIST"
     SPELLWEAVER = "SPELLWEAVER"
     BLOODBOUND = "BLOODBOUND"
     SHADE = "SHADE"
     MACHINIST = "MACHINIST"
     SOULFORGED = "SOULFORGED"
     GAMBLER = "GAMBLER"
     OATHBOUND = "OATHBOUND"
     UNKNOWN = "UNKNOWN"

class AuxiliaryCardType(models.TextChoices):
    NONE = "NONE",
    REAGENT = "REAGENT",
    OATH = "OATH",
    CONSUMABLE = "CONSUMABLE",
    FRAGMENT = "FRAGMENT",
    RELIC = "RELIC"
    TEMPLATE = "TEMPLATE"

class BannerType(models.TextChoices):
    CARD = "CARD", "Card"
    EQUIP = "EQUIP", "Equipment"
    CHARACTER = "CHARACTER", "Character"

class CurrencyType(models.TextChoices):
    STANDARD_PULL = "STANDARD_PULL", "Standard Pull Currency"
    LIMITED_PULL = "LIMITED_PULL", "Limited Pull Currency"


from enum import Enum

class DynamicValueType(str, Enum):
    STATIC_NUMBER = "static_number"  # Optional placeholder
    YOUR_CARDS_IN_HAND = "your_cards_in_hand"
    ENEMY_CARDS_IN_HAND = "enemy_cards_in_hand"
    FRIENDLY_BOARD_COUNT = "friendly_board_count"
    ENEMY_BOARD_COUNT = "enemy_board_count"
    FRIENDLY_GRAVEYARD_COUNT = "friendly_graveyard_count"
    ENEMY_GRAVEYARD_COUNT = "enemy_graveyard_count"
    TURN_NUMBER = "turn_number"
    OWNER_HEALTH = "owner_health"
    OPPONENT_HEALTH = "opponent_health"

    @classmethod
    def choices(cls):
        numeric_choices = [(str(i), str(i)) for i in range(0, 21)]
        enum_choices = [(v.value, v.name.replace("_", " ").title()) for v in cls if v != cls.STATIC_NUMBER]
        return numeric_choices + enum_choices