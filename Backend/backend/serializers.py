
from rest_framework import serializers
from core.models import Card, Deck, Character, DeckCard, User, Effect, Condition, Trigger, CardEffectBinding, Subtype, Banner, BannerItem

class CardSerializer(serializers.ModelSerializer):
    class Meta:
        model = Card
        fields = '__all__'

class DeckSerializer(serializers.ModelSerializer):
    class Meta:
        model = Deck
        fields = '__all__'

class CharacterSerializer(serializers.ModelSerializer):
    class Meta:
        model = Character
        fields = '__all__'


class UserSerializer(serializers.ModelSerializer):
    class Meta:
        model = User
        fields = ['id', 'username', 'rank']

class UserDataSerializer(serializers.ModelSerializer):
    class Meta:
        model = User
        fields = ['user_data']
        
class DeckCardSerializer(serializers.ModelSerializer):
    class Meta:
        model = DeckCard
        fields = '__all__'

class TriggerSerializer(serializers.ModelSerializer):
    class Meta:
        model = Trigger
        fields = '__all__'

class EffectSerializer(serializers.ModelSerializer):
    class Meta:
        model = Effect
        fields = '__all__'

class ConditionSerializer(serializers.ModelSerializer):
    class Meta:
        model = Condition
        fields = '__all__'

class CardEffectBindingSerializer(serializers.ModelSerializer):
    class Meta:
        model = CardEffectBinding
        fields = '__all__'

class SubtypeSerializer(serializers.ModelSerializer):
    class Meta:
        model = Subtype
        fields = '__all__'



class BannerItemSerializer(serializers.ModelSerializer):
    card = CardSerializer(read_only=True)
    card_id = serializers.PrimaryKeyRelatedField(
        queryset=Card.objects.all(), source='card', write_only=True
    )

    class Meta:
        model = BannerItem
        fields = ['id', 'card', 'card_id', 'is_featured']


class BannerSerializer(serializers.ModelSerializer):
    items = BannerItemSerializer(many=True, read_only=True)

    featured_legendary = CardSerializer(read_only=True)
    featured_legendary_id = serializers.PrimaryKeyRelatedField(
        queryset=Card.objects.all(), source='featured_legendary', write_only=True, required=False
    )

    class Meta:
        model = Banner
        fields = [
            'id', 'name', 'type', 'pull_currency', 'is_limited',
            'start_time', 'end_time', 'hard_pity', 'soft_pity_start',
            'featured_legendary', 'featured_legendary_id', 'items'
        ]
