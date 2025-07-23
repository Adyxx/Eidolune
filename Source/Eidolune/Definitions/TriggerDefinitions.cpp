#include "TriggerRegistry.h"


void RegisterTriggerDefinitions() {

    TriggerInfo onCardPlayed;
    onCardPlayed.Name = "OnCardPlayed";
    onCardPlayed.Description = "Triggered when a card is played.";

    TriggerInfo onDamageTaken;
    onDamageTaken.Name = "OnDamageTaken";
    onDamageTaken.Description = "Triggered when damage is taken.";

    TriggerRegistry::Register("on_card_played", onCardPlayed);
    TriggerRegistry::Register("on_damage_taken", onDamageTaken);


}
