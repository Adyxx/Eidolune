from django.contrib import admin
from .models.character import Character
from .models.relationship import CharacterRelationship
from .models.deck import Deck
from .models.deckcard import DeckCard
from .models.card import Card, Faction, Subtype
from .models.effect import Effect
from .models.trigger import Trigger
from .models.condition import Condition
from .models.cardeffectbinding import CardEffectBinding
from django.contrib import messages
from .services.deck_service import get_deck_issues
from django.contrib import admin
from django.contrib.auth.admin import UserAdmin as BaseUserAdmin
from .models.users import User

class DeckAdmin(admin.ModelAdmin):
    def save_model(self, request, obj, form, change):
        issues = get_deck_issues(obj)
        if issues:
            messages.warning(request, "Deck has issues:\n" + "\n".join(issues))
        super().save_model(request, obj, form, change)

@admin.register(Card)
class CardAdmin(admin.ModelAdmin):
    list_display = ("name", "rarity", "type", "cost")
    list_filter = ("rarity", "type", "faction")
    search_fields = ("name",)

admin.site.register(Faction)
admin.site.register(Subtype)
admin.site.register(Effect)
admin.site.register(Trigger)
admin.site.register(Condition)
admin.site.register(CardEffectBinding)

@admin.register(Character)
class CharacterAdmin(admin.ModelAdmin):
    list_display = ("name", "class_type", "gender", "health")
    list_filter = ("gender", "class_type", "faction")
    search_fields = ("name",)

admin.site.register(CharacterRelationship)
admin.site.register(Deck)
admin.site.register(DeckCard)

@admin.register(User)
class UserAdmin(BaseUserAdmin):
    fieldsets = BaseUserAdmin.fieldsets + (
        ("Custom Fields", {"fields": ("rank",)}),
    )

    list_display = ("username", "email", "rank", "is_staff", "is_active")