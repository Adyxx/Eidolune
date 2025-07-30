
from rest_framework import viewsets
from core.models import Card, Deck, Character, DeckCard, Effect, Condition, Trigger, CardEffectBinding
from .serializers import CardSerializer, DeckSerializer, CharacterSerializer, UserSerializer, DeckCardSerializer, TriggerSerializer, EffectSerializer, ConditionSerializer, CardEffectBindingSerializer
from core.models import User

class CardViewSet(viewsets.ReadOnlyModelViewSet):
    queryset = Card.objects.all()
    serializer_class = CardSerializer

class DeckViewSet(viewsets.ReadOnlyModelViewSet):
    queryset = Deck.objects.all()
    serializer_class = DeckSerializer

class CharacterViewSet(viewsets.ReadOnlyModelViewSet):
    queryset = Character.objects.all()
    serializer_class = CharacterSerializer

class UserViewSet(viewsets.ReadOnlyModelViewSet):
    queryset = User.objects.all()
    serializer_class = UserSerializer

class DeckCardViewSet(viewsets.ReadOnlyModelViewSet):
    queryset = DeckCard.objects.all()
    serializer_class = DeckCardSerializer

class TriggerViewSet(viewsets.ReadOnlyModelViewSet):
    queryset = Trigger.objects.all()
    serializer_class = TriggerSerializer

class EffectViewSet(viewsets.ReadOnlyModelViewSet):
    queryset = Effect.objects.all()
    serializer_class = EffectSerializer

class ConditionViewSet(viewsets.ReadOnlyModelViewSet):
    queryset = Condition.objects.all()
    serializer_class = ConditionSerializer

class CardEffectBindingViewSet(viewsets.ReadOnlyModelViewSet):
    queryset = CardEffectBinding.objects.all()
    serializer_class = CardEffectBindingSerializer
