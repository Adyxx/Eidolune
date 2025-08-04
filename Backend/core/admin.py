from django.contrib import admin
from django.contrib import messages
from django.contrib.auth.admin import UserAdmin as BaseUserAdmin
from .models.character import Character
from .models.relationship import CharacterRelationship
from .models.deck import Deck
from .models.deckcard import DeckCard
from .models.card import Card, Faction, Subtype
from .models.effect import Effect
from .models.trigger import Trigger
from .models.condition import Condition
from .models.cardeffectbinding import CardEffectBinding
from .models.users import User
from .services.deck_service import get_deck_issues
from .models.banner import Banner, BannerItem


### INLINE ADMIN CLASSES ###

class CardEffectBindingInline(admin.TabularInline):
    model = CardEffectBinding
    extra = 1


class DeckCardInline(admin.TabularInline):
    model = DeckCard
    extra = 1


### DECK ADMIN ###

@admin.register(Deck)
class DeckAdmin(admin.ModelAdmin):
    inlines = [DeckCardInline]

    def save_model(self, request, obj, form, change):
        issues = get_deck_issues(obj)
        if issues:
            messages.warning(request, "Deck has issues:\n" + "\n".join(issues))
        super().save_model(request, obj, form, change)


### CARD ADMIN ###

@admin.register(Card)
class CardAdmin(admin.ModelAdmin):
    list_display = ("name", "rarity", "type", "cost")
    list_filter = ("rarity", "type", "faction")
    search_fields = ("name",)
    inlines = [CardEffectBindingInline]
    filter_horizontal = ("subtypes",)


### CHARACTER ADMIN ###

@admin.register(Character)
class CharacterAdmin(admin.ModelAdmin):
    list_display = ("name", "class_type", "gender", "health")
    list_filter = ("gender", "class_type", "faction")
    search_fields = ("name",)
    filter_horizontal = ("subtypes",)


### REMAINING MODELS ###

admin.site.register(Faction)
admin.site.register(Subtype)
admin.site.register(Effect)
admin.site.register(Trigger)
admin.site.register(Condition)

admin.site.register(CardEffectBinding)
admin.site.register(CharacterRelationship)


### CUSTOM USER ADMIN ###

@admin.register(User)
class UserAdmin(BaseUserAdmin):
    fieldsets = BaseUserAdmin.fieldsets + (
        ("Custom Fields", {"fields": ("rank",)}),
    )
    list_display = ("username", "email", "rank", "is_staff", "is_active")

class BannerItemInline(admin.TabularInline):
    model = BannerItem
    extra = 1
    autocomplete_fields = ['card']
    show_change_link = True
    readonly_fields = ['card_rarity']
    fields = ['card', 'card_rarity', 'is_featured']

    def card_rarity(self, obj):
        return obj.card.rarity if obj.card else "-"
    card_rarity.short_description = "Rarity"

@admin.register(Banner)
class BannerAdmin(admin.ModelAdmin):
    inlines = [BannerItemInline]
