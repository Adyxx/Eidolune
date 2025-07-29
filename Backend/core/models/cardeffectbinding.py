from django.db import models
from .card import Card
from .trigger import Trigger
from .effect import Effect
from .condition import Condition
from .enums import TargetSpec, ListeningZone, TriggerScope



class CardEffectBinding(models.Model):
    card = models.ForeignKey(Card, on_delete=models.CASCADE, related_name="bindings")
    trigger = models.ForeignKey(Trigger, on_delete=models.CASCADE)
    effect = models.ForeignKey(Effect, on_delete=models.CASCADE)
    condition = models.ForeignKey(Condition, on_delete=models.SET_NULL, null=True, blank=True)

    value = models.IntegerField(null=True, blank=True)
    targeting = models.CharField(max_length=30, choices=TargetSpec.choices, null=True, blank=True)

    zone = models.CharField(max_length=30, choices=ListeningZone.choices, default=ListeningZone.ANY)
    scope = models.CharField(max_length=30, choices=TriggerScope.choices, default=TriggerScope.SELF)