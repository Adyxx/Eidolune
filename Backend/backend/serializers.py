
from rest_framework import serializers
from core.models import Card, Deck, Character, DeckCard, User, Effect, Condition, Trigger, CardEffectBinding, Subtype

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
