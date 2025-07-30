from django.db import models

class Effect(models.Model):
    name = models.CharField(max_length=100)
    script_reference = models.CharField(max_length=100)
    description = models.TextField(blank=True)
    requires_value = models.BooleanField(default=False)
    requires_target = models.BooleanField(default=False)

    def __str__(self):
        return self.script_reference