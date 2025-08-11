
from django import forms
from .models.cardeffectbinding import CardEffectBinding

class CardEffectBindingForm(forms.ModelForm):
    class Meta:
        model = CardEffectBinding
        fields = "__all__"

    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)

        if self.instance and self.instance.keyword_template:
            kt = self.instance.keyword_template
            if not self.instance.pk:  # Only prefill on new rows
                self.initial.update({
                    "trigger": kt.trigger,
                    "effect": kt.effect,
                    "value": kt.value,
                    "targeting": kt.targeting,
                    "targeting_rule": kt.targeting_rule,
                    "condition": kt.condition,
                    "condition_value": kt.condition_value,
                    "zone": kt.zone,
                    "scope": kt.scope,
                    "linked_card": kt.linked_card,
                })
