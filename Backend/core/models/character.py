from django.db import models
from .enums import Gender, CharacterClassType
from .card import Faction, Subtype

class Character(models.Model):
    name = models.CharField(max_length=100)
    age = models.IntegerField(default=0)
    height_cm = models.FloatField(default=0.0)
    gender = models.CharField(max_length=20, choices=Gender.choices, default=Gender.UNKNOWN)

    birth_date = models.CharField(max_length=20, blank=True)
    short_bio = models.TextField(blank=True)
    backstory = models.TextField(blank=True)

    image_path = models.CharField(max_length=255, blank=True)
    model_file_path = models.CharField(max_length=255, blank=True)

    faction = models.ForeignKey(Faction, on_delete=models.SET_NULL, null=True, blank=True)
    subtypes = models.ManyToManyField(Subtype, blank=True)

    class_type = models.CharField(max_length=30, choices=CharacterClassType.choices, default=CharacterClassType.UNKNOWN)

    health = models.IntegerField(default=0)
    #partner_hp = models.IntegerField(null=True, blank=True)

    passive_ability_ref = models.CharField(max_length=100, blank=True)
    passive_description = models.TextField(blank=True)

    active_ability_ref = models.CharField(max_length=100, blank=True)
    active_description = models.TextField(blank=True)

    partner_ability_ref = models.CharField(max_length=100, blank=True)
    partner_description = models.TextField(blank=True)

    solo_bonus_ref = models.CharField(max_length=100, blank=True)
    solo_bonus_description = models.TextField(blank=True)

    deck_restriction_ref = models.CharField(max_length=100, blank=True)
    deck_restriction_description = models.TextField(blank=True)

    def __str__(self):
        return self.name
