from django.db import models
from .enums import Rarity, CardType
from .faction import Faction
from .subtype import Subtype


class Card(models.Model):
    name = models.CharField(max_length=100)
    image_path = models.CharField(max_length=200)
    cost = models.IntegerField()
    rarity = models.CharField(max_length=20, choices=Rarity.choices)
    type = models.CharField(max_length=20, choices=CardType.choices)
    power = models.IntegerField(null=True, blank=True)
    health = models.IntegerField(null=True, blank=True)

    faction = models.ForeignKey(Faction, on_delete=models.SET_NULL, null=True, blank=True)
    subtypes = models.ManyToManyField(Subtype, blank=True)

    is_character_card = models.BooleanField(default=False)
    is_character_exclusive = models.BooleanField(default=False)

    text = models.TextField(blank=True)

    def __str__(self):
        return self.name
