#include "TriggerRegistry.h"
#include "TriggerDefinitions.h"

void RegisterTriggerDefinitions() {
    auto& registry = TriggerRegistry::Instance();

    // Trigger: OnPlay
    TriggerInfo onPlay;
    onPlay.Name = "OnPlay";
    onPlay.Description = "When this card is played.";
    onPlay.Event = "card_played";

    auto onPlayTrigger = std::make_shared<Trigger>("card_played");

    //registry.RegisterInfo("on_play", onPlay);
    registry.RegisterShared("on_play", onPlayTrigger);
    registry.RegisterShared(1, onPlayTrigger);

    // Trigger: OnDeath
    TriggerInfo onDeath;
    onDeath.Name = "OnDeath";
    onDeath.Description = "When this card dies.";
    onDeath.Event = "card_died";

    auto onDeathTrigger = std::make_shared<Trigger>("card_died");

    //registry.RegisterInfo("on_death", onDeath);
    registry.RegisterShared("on_death", onDeathTrigger);
    registry.RegisterShared(2, onDeathTrigger);
}



/*

enum BuiltInTriggerId {
    TRIGGER_ON_PLAY = 1,
    TRIGGER_ON_DEATH = 2
};

registry.RegisterShared(TRIGGER_ON_PLAY, onPlayTrigger);

*/