from django.db import models


class Subtype(models.Model):
    name = models.CharField(max_length=50)