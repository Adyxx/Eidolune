from django.db import models
from django.contrib.auth import get_user_model
from core.models.character import Character
from django.contrib.postgres.fields import ArrayField

User = get_user_model()

class UserCharacterData(models.Model):
    user = models.ForeignKey(User, on_delete=models.CASCADE, related_name="owned_characters")
    character = models.ForeignKey(Character, on_delete=models.CASCADE)
    
    level = models.IntegerField(default=1)
    equipped = models.JSONField(default=dict)
    consumables = ArrayField(models.IntegerField(), default=list)

    def __str__(self):
        return f"{self.user.username}'s {self.character.name} (Level {self.level})"

    def to_json(self):
        return {
            "character_id": self.character.id,
            "level": self.level,
            "equipped": self.equipped,
            "consumables": self.consumables
        }
