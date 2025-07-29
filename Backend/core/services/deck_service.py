
from ..models.deck import Deck
from typing import List

def total_card_count(deck: Deck) -> int:
    return sum(dc.quantity for dc in deck.deck_cards.all())

def get_deck_issues(deck: Deck) -> List[str]:
    issues = []
    count = total_card_count(deck)
    if count < 30:
        issues.append(f"Too few cards ({count}). Minimum is 30.")
    elif count > 60:
        issues.append(f"Too many cards ({count}). Maximum is 60.")

    if deck.main_character == deck.partner_character:
        issues.append("Main and partner characters are the same.")

    if deck.main_character and not deck.main_character.class_type:
        issues.append("Main character has no class.")

    return issues

def is_deck_playable(deck: Deck) -> bool:
    return len(get_deck_issues(deck)) == 0