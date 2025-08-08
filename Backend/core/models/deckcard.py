from django.db import models
from .deck import Deck
from .card import Card
from django.core.exceptions import ValidationError
from .enums import AuxiliaryCardType

class DeckCard(models.Model):
    deck = models.ForeignKey(Deck, on_delete=models.CASCADE, related_name="deck_cards")
    card = models.ForeignKey(Card, on_delete=models.CASCADE)
    quantity = models.IntegerField(default=1)

    class Meta:
        unique_together = ("deck", "card")

    def clean(self):
            if self.card.auxiliarytype != AuxiliaryCardType.NONE:
                raise ValidationError(f"Cannot include non-playable card ({self.card.name}) of type {self.card.auxiliarytype} in a deck.")
    
    def __str__(self):
        return f"{self.quantity}x {self.card.name} in {self.deck.name}"
