from django.db import models

class Trigger(models.Model):
    script_reference = models.CharField(max_length=100)
    description = models.TextField(blank=True)