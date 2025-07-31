from django.db import models

class Faction(models.Model):
    name = models.CharField(max_length=50)
    color = models.CharField(max_length=20, blank=True)


    def __str__(self):
        return self.name
