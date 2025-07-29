# services/character_service.py

from ..models.character import Character
from ..models.relationship import CharacterRelationship

def get_partners(character: Character):
    return CharacterRelationship.objects.filter(
        source=character,
        type="PARTNER"
    ).select_related("target")
