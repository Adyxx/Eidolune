
from ..models.deck import Deck
from ..models.card import Card
from ..models.character import Character

def export_card(card: Card) -> dict:
    return {
        "id": str(card.id),
        "name": card.name,
        "cost": card.cost,
        "rarity": card.rarity,
        "type": card.type,
        "text": card.text,
        "power": card.power,
        "health": card.health,
    }

def export_deck(deck: Deck) -> dict:
    return {
        "name": deck.name,
        "mainCharacter": deck.main_character.name if deck.main_character else None,
        "partnerCharacter": deck.partner_character.name if deck.partner_character else None,
        "cards": [
            {
                "card_id": dc.card.id,
                "name": dc.card.name,
                "quantity": dc.quantity
            }
            for dc in deck.deck_cards.all()
        ]
    }
