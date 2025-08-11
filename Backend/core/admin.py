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
from .models.banner import Banner, BannerItem, BannerItemForm
from .models.enums import AuxiliaryCardType
from .models.keywordeffecttemplate import KeywordEffectTemplate
from .forms import CardEffectBindingForm
from django.http import JsonResponse
from django.urls import path


### INLINE ADMIN CLASSES ###

class CardEffectBindingInline(admin.TabularInline):
    model = CardEffectBinding
    extra = 1
    fk_name = 'card'
    #form = CardEffectBindingForm

    class Media:
        js = (
            'admin/js/vendor/jquery/jquery.min.js',
            'js/fill_keyword.js',
        )

class DeckCardInline(admin.TabularInline):
    model = DeckCard
    extra = 1

@admin.register(KeywordEffectTemplate)
class KeywordEffectTemplateAdmin(admin.ModelAdmin):
    list_display = ("name", "trigger", "effect", "zone", "scope")
    search_fields = ("name",)

### CARD ADMIN ###

@admin.register(Card)
class CardAdmin(admin.ModelAdmin):
    list_display = ("name", "rarity", "type", "cost")
    list_filter = ("rarity", "type", "faction")
    search_fields = ("name",)
    inlines = [CardEffectBindingInline]
    filter_horizontal = ("subtypes",)
    #filter_horizontal = ("subtypes", "keywords")
    '''

    def save_related(self, request, form, formsets, change):
        super().save_related(request, form, formsets, change)

        card = form.instance
        for keyword in card.keywords.all():
            if not CardEffectBinding.objects.filter(card=card, trigger=keyword.trigger, effect=keyword.effect).exists():
                CardEffectBinding.objects.create(
                    card=card,
                    trigger=keyword.trigger,
                    effect=keyword.effect,
                    value=keyword.value,
                    targeting=keyword.targeting,
                    targeting_rule=keyword.targeting_rule,
                    condition=keyword.condition,
                    condition_value=keyword.condition_value,
                    zone=keyword.zone,
                    scope=keyword.scope,
                    linked_card=keyword.linked_card
                )
    
    '''

    def get_search_results(self, request, queryset, search_term):
        if request.GET.get('_banner_autocomplete'):
            queryset = queryset.filter(auxiliarytype=AuxiliaryCardType.NONE)
        return super().get_search_results(request, queryset, search_term)

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
    form = BannerItemForm
    autocomplete_fields = ['card']
    show_change_link = True
    readonly_fields = ['card_rarity']
    fields = ['card', 'card_rarity', 'is_featured']

    def formfield_for_foreignkey(self, db_field, request=None, **kwargs):
        formfield = super().formfield_for_foreignkey(db_field, request, **kwargs)
        if db_field.name == 'card' and hasattr(formfield.widget, 'attrs'):
            formfield.widget.attrs['data-ajax--params'] = '{"_banner_autocomplete": 1}'
        return formfield

    def card_rarity(self, obj):
        return obj.card.rarity if obj.card else "-"
    card_rarity.short_description = "Rarity"

@admin.register(Banner)
class BannerAdmin(admin.ModelAdmin):
    inlines = [BannerItemInline]
