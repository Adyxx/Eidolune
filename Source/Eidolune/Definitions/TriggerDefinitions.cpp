#include "TriggerRegistry.h"
#include "TriggerDefinitions.h"

/*
void RegisterTriggerDefinitions() {

    TriggerInfo onCardPlayed;
    onCardPlayed.Name = "OnCardPlayed";
    onCardPlayed.Description = "Triggered when a card is played.";

    TriggerInfo onDamageTaken;
    onDamageTaken.Name = "OnDamageTaken";
    onDamageTaken.Description = "Triggered when damage is taken.";

    TriggerRegistry::Register("on_play", onCardPlayed);
    TriggerRegistry::Register("on_damage_taken", onDamageTaken);


}
*/

void RegisterTriggerDefinitions() {

    TriggerInfo onPlay;
    TriggerInfo onDeath;

    onPlay.Name = "OnPlay";
    onPlay.Description = "When this card is played.";
    onPlay.Event = "card_played";


    onDeath.Name = "OnDeath";
    onDeath.Description = "When this card dies.";
    onDeath.Event = "card_died";

    TriggerRegistry::Register("on_play", onPlay);
    TriggerRegistry::Register("on_death", onDeath);
}
