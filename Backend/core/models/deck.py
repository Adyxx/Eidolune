from django.db import models
from .character import Character
from .card import Card
from .users import User

class Deck(models.Model):
    owner = models.ForeignKey(User, on_delete=models.CASCADE)
    name = models.CharField(max_length=100)
    description = models.TextField(blank=True)

    main_character = models.ForeignKey(Character, on_delete=models.CASCADE, related_name="decks_main")
    partner_character = models.ForeignKey(Character, on_delete=models.SET_NULL, null=True, blank=True, related_name="decks_partner")

    def __str__(self):
        return f"{self.name} ({self.owner.username})"

