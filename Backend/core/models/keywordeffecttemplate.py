from django.db import models
from .trigger import Trigger
from .effect import Effect
from .condition import Condition
from .enums import TargetSpec, ListeningZone, TriggerScope, DynamicValueType, TargetingRule

class KeywordEffectTemplate(models.Model):
    name = models.CharField(max_length=50, unique=True)  # e.g. "LIFELINK", "PIERCE"
    trigger = models.ForeignKey(Trigger, on_delete=models.CASCADE)
    effect = models.ForeignKey(Effect, on_delete=models.CASCADE)
    value = models.CharField(
        max_length=50,
        choices=DynamicValueType.choices(),
        null=True,
        blank=True
    )
    targeting = models.CharField(max_length=30, choices=TargetSpec.choices, null=True, blank=True)
    targeting_rule = models.CharField(max_length=30, choices=TargetingRule.choices, null=True, blank=True)
    condition = models.ForeignKey(Condition, on_delete=models.SET_NULL, null=True, blank=True)
    condition_value = models.IntegerField(null=True, blank=True)

    zone = models.CharField(max_length=30, choices=ListeningZone.choices, default=ListeningZone.ANY)
    scope = models.CharField(max_length=30, choices=TriggerScope.choices, default=TriggerScope.SELF)

    linked_card = models.ForeignKey(
        "Card", on_delete=models.SET_NULL, null=True, blank=True, related_name="keyword_linked_bindings"
    )

    def __str__(self):
        return f"{self.name}"

