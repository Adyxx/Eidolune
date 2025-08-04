from django.db import models
from .enums import BannerType, CurrencyType
from .card import Card
from django import forms
from .enums import AuxiliaryCardType

class Banner(models.Model):
    name = models.CharField(max_length=100)

    type = models.CharField(max_length=20, choices=BannerType.choices)
    pull_currency = models.CharField(max_length=20, choices=CurrencyType.choices)

    is_limited = models.BooleanField(default=True)

    start_time = models.DateTimeField(null=True, blank=True)
    end_time = models.DateTimeField(null=True, blank=True)

    hard_pity = models.IntegerField(default=80)
    soft_pity_start = models.IntegerField(default=60)

    featured_legendary = models.ForeignKey(
        Card, null=True, blank=True, on_delete=models.SET_NULL, related_name="featured_legendary_in"
    )

    def __str__(self):
        return f"{self.name} ({'Limited' if self.is_limited else 'Standard'})"


class BannerItem(models.Model):
    banner = models.ForeignKey(Banner, on_delete=models.CASCADE, related_name="items")
    card = models.ForeignKey(Card, on_delete=models.CASCADE)

    # Only applies to non-legendary featured cards
    is_featured = models.BooleanField(default=False)

    class Meta:
        unique_together = ('banner', 'card') 

    def __str__(self):
        return f"{self.card.name} in {self.banner.name}"
    


class BannerItemForm(forms.ModelForm):
    class Meta:
        model = BannerItem
        fields = '__all__'

    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)

        # Filter to only show cards with auxilarytype = NONE
        self.fields['card'].queryset = Card.objects.filter(auxilarytype=AuxiliaryCardType.NONE)
