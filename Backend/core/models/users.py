# core/models/users.py
from django.contrib.auth.models import AbstractUser
from django.db import models

class User(AbstractUser):
    rank = models.IntegerField(default=0)
    user_data = models.JSONField(default=dict)

    def __str__(self):
        return self.username
