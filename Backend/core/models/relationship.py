from django.db import models
from .character import Character
from .enums import RelationshipType

class CharacterRelationship(models.Model):
    source = models.ForeignKey(Character, on_delete=models.CASCADE, related_name="relationships_outgoing")
    target = models.ForeignKey(Character, on_delete=models.CASCADE, related_name="relationships_incoming")
    type = models.CharField(max_length=30, choices=RelationshipType.choices)
    description = models.TextField(blank=True)

    class Meta:
        unique_together = ("source", "target", "type")

    def __str__(self):
        return f"{self.source} → {self.type} → {self.target}"
