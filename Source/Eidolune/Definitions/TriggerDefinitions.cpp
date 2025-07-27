#include "TriggerRegistry.h"
#include "TriggerDefinitions.h"

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
