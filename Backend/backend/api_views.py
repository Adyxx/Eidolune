
from rest_framework import viewsets
from core.models import Card, Deck, Character, DeckCard, Effect, Condition, Trigger, CardEffectBinding, Subtype
from .serializers import CardSerializer, DeckSerializer, CharacterSerializer, SubtypeSerializer, UserSerializer, DeckCardSerializer, TriggerSerializer, EffectSerializer, ConditionSerializer, CardEffectBindingSerializer
from core.models import User
from core.models import KeywordEffectTemplate
from .serializers import KeywordEffectTemplateSerializer


from rest_framework.views import APIView
from rest_framework.response import Response
from rest_framework import status
from django.shortcuts import get_object_or_404
from rest_framework.decorators import action
from core.models import Banner, BannerItem
from .serializers import BannerSerializer, BannerItemSerializer
from django.utils.timezone import now
from rest_framework.decorators import action
from rest_framework.response import Response
from django.db import models

class CardViewSet(viewsets.ReadOnlyModelViewSet):
    queryset = Card.objects.all()
    serializer_class = CardSerializer

class DeckViewSet(viewsets.ReadOnlyModelViewSet):
    queryset = Deck.objects.all()
    serializer_class = DeckSerializer

class CharacterViewSet(viewsets.ReadOnlyModelViewSet):
    queryset = Character.objects.all()
    serializer_class = CharacterSerializer

from .serializers import UserSerializer, UserDataSerializer

class UserViewSet(viewsets.ReadOnlyModelViewSet):
    queryset = User.objects.all()
    serializer_class = UserSerializer

    @action(detail=True, methods=["get", "post"], url_path='data')
    def user_data(self, request, pk=None):
        user = self.get_object()

        if request.method == "GET":
            return Response(user.user_data or {})

        if request.method == "POST":
            print("Saving user_data:", request.data)
            user.user_data = request.data
            user.save()
            return Response({"message": "User data saved."})

        
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

class SubtypeViewSet(viewsets.ReadOnlyModelViewSet):
    queryset = Subtype.objects.all()
    serializer_class = SubtypeSerializer


class BannerViewSet(viewsets.ModelViewSet):
    queryset = Banner.objects.all()
    serializer_class = BannerSerializer

    @action(detail=False, methods=['get'], url_path='active')
    def active_banners(self, request):
        now_time = now()
        active = self.queryset.filter(
            models.Q(is_limited=False) | #always shows non-limited banners
            models.Q(start_time__lte=now_time, end_time__gte=now_time)
        )
        serializer = self.get_serializer(active, many=True)
        return Response(serializer.data)
    
class BannerItemViewSet(viewsets.ModelViewSet):
    queryset = BannerItem.objects.all()
    serializer_class = BannerItemSerializer


class KeywordEffectTemplateViewSet(viewsets.ReadOnlyModelViewSet):
    queryset = KeywordEffectTemplate.objects.all()
    serializer_class = KeywordEffectTemplateSerializer