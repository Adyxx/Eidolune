

def get_deck_issues(deck) -> list[str]:
    issues = []
    '''
    if deck.total_card_count() < 40:
        issues.append("Deck has fewer than 40 cards.")

    card_counts = {}
    character_card_tracker = {}
    character_card_found = set()

    for deck_card in deck.deck_cards.select_related("card__character", "card__effect_bindings__effect", "card__effect_bindings__trigger"):
        card = deck_card.card
        count = deck_card.quantity

        limit = 1 if card.is_character_card else 3

        for binding in card.effect_bindings.filter(trigger__script_reference="on_deck_build"):
            if binding.effect.script_reference == "override_deck_limit":
                try:
                    new_limit = int(binding.value)
                    limit = new_limit
                except (ValueError, TypeError):
                    issues.append(f"Invalid override_deck_limit value on card '{card.name}'")

        card_counts[card.id] = card_counts.get(card.id, 0) + count
        if card_counts[card.id] > limit:
            issues.append(f"{card.name} exceeds allowed limit of {limit} copies.")

        if card.is_character_card and card.character:
            cid = card.character.id
            if cid in character_card_tracker:
                issues.append(f"Multiple character cards for '{card.character.name}' in deck.")
            else:
                character_card_tracker[cid] = True
                character_card_found.add(cid)

        if card.is_character_exclusive and card.character:
            allowed = {deck.main_character.id if deck.main_character else None}
            if deck.partner_character:
                allowed.add(deck.partner_character.id)
            if card.character.id not in allowed:
                issues.append(f"{card.name} is exclusive to '{card.character.name}', who is not part of this deck.")

    if deck.main_character and deck.main_character.id not in character_card_found:
        issues.append(f"Deck is missing character card for '{deck.main_character.name}'.")

    if deck.partner_character and deck.partner_character.id not in character_card_found:
        issues.append(f"Deck is missing character card for partner '{deck.partner_character.name}'.")

    if deck.main_character and deck.main_character.deck_restriction_ref:
        from backend.registry.deck_restrictions import DECK_RESTRICTION_REGISTRY
        func = DECK_RESTRICTION_REGISTRY.get(deck.main_character.deck_restriction_ref)
        if func:
            is_valid, reason = func(deck)
            if not is_valid:
                issues.append(f"Main character deck restriction: {reason}")

    if deck.partner_character and deck.partner_character.deck_restriction_ref:
        func = DECK_RESTRICTION_REGISTRY.get(deck.partner_character.deck_restriction_ref)
        if func:
            is_valid, reason = func(deck)
            if not is_valid:
                issues.append(f"Partner deck restriction: {reason}")
    '''
    return issues
