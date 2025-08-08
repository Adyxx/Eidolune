from django.db import models
from .enums import Rarity, CardType, CharacterClassType, AuxiliaryCardType
from .faction import Faction
from .subtype import Subtype
from .character import Character
from django.core.exceptions import ValidationError

class Card(models.Model):
    name = models.CharField(max_length=100)
    image_path = models.CharField(null=True, blank=True, max_length=200)
    cost = models.IntegerField(null=True, blank=True)
    rarity = models.CharField(max_length=20, choices=Rarity.choices, null=True, blank=True)
    type = models.CharField(max_length=20, choices=CardType.choices, null=True, blank=True)
    power = models.IntegerField(null=True, blank=True)
    health = models.IntegerField(null=True, blank=True)

    faction = models.ForeignKey(Faction, on_delete=models.SET_NULL, null=True, blank=True)
    subtypes = models.ManyToManyField(Subtype, blank=True)

    is_character_card = models.BooleanField(default=False)
    is_character_exclusive = models.BooleanField(default=False)

    text = models.TextField(blank=True)
    
    character = models.ForeignKey(Character, null=True, blank=True, on_delete=models.SET_NULL, related_name='cards')

    classlock = models.CharField(
        max_length=20,
        choices=CharacterClassType.choices,
        default=CharacterClassType.UNKNOWN
    )
    auxiliarytype = models.CharField(
        max_length=20,
        choices=AuxiliaryCardType.choices,
        default=AuxiliaryCardType.NONE
    )
    
    def clean(self):
        if self.auxiliarytype != AuxiliaryCardType.TEMPLATE:
            if self.cost is None:
                raise ValidationError("Non-template cards must have a cost.")
            if not self.rarity:
                raise ValidationError("Non-template cards must have a rarity.")
            if not self.type:
                raise ValidationError("Non-template cards must have a type.")

        if self.is_character_card and not self.character:
            raise ValidationError("Character card must have a character assigned.")

        if not self.is_character_card and self.is_character_exclusive and not self.character:
            raise ValidationError("Character-exclusive cards must have a character assigned.")

        if not self.is_character_card and self.character and not self.is_character_exclusive:
            raise ValidationError("Non-character cards should not have a character unless marked exclusive.")

    def __str__(self):
        return self.name
