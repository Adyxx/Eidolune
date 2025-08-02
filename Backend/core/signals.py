# core/signals.py
from django.db.models.signals import post_save
from django.dispatch import receiver
from django.conf import settings
from core.models.deck import Deck
from core.models.deckcard import DeckCard
from core.models.character import Character
from core.models.card import Card
from core.models.usercharacterdata import UserCharacterData

DEFAULT_CHARACTER_ID = 1
DEFAULT_CARDS = [(1, 10), (2, 10), (3, 10)]

@receiver(post_save, sender=settings.AUTH_USER_MODEL)
def create_default_user_data(sender, instance, created, **kwargs):
    if not created:
        return

    print(f"🛠 Creating default data for user: {instance.username}")

    if Deck.objects.filter(owner=instance).exists():
        print("⚠️ User already has decks, skipping default data creation.")
        return

    starter_character = Character.objects.get(id=DEFAULT_CHARACTER_ID)

    UserCharacterData.objects.create(
        user=instance,
        character=starter_character,
        level=1,
        consumables=[],
        equipped={},
    )

    deck = Deck.objects.create(
        owner=instance,
        name="Starter Deck",
        main_character=starter_character,
        partner_character=None,
        description="Your first deck!"
    )

    for card_id, quantity in DEFAULT_CARDS:
        card = Card.objects.get(id=card_id)
        DeckCard.objects.create(deck=deck, card=card, quantity=quantity)

    from collections import defaultdict

    # Build decks with embedded cards
    deck_data = []
    for deck in instance.decks.all():
        cards = DeckCard.objects.filter(deck=deck).values("card_id", "quantity")
        deck_data.append({
            "id": deck.id,
            "name": deck.name,
            "main_character": deck.main_character_id,
            "partner_character": deck.partner_character_id,
            "cards": list(cards),
        })

    # Final user_data payload
    instance.user_data = {
        "user_id": instance.id,
        "decks": deck_data,
        "characters": list(
            UserCharacterData.objects.filter(user=instance).values(
                "character_id", "level", "equipped", "consumables"
            )
        )
    }
    
    instance.save(update_fields=["user_data"])

    print("✅ Default user data created.")