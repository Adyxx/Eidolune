#include "TriggerRegistry.h"
#include "TriggerDefinitions.h"

void RegisterTriggerDefinitions() {
    auto& registry = TriggerRegistry::Instance();

    TriggerInfo onPlay;
    onPlay.Name = "OnPlay";
    onPlay.Description = "When this card is played.";
    onPlay.Event = "card_played";

    auto onPlayTrigger = std::make_shared<Trigger>("card_played");

    registry.RegisterShared("on_play", onPlayTrigger);
    //registry.RegisterShared(1, onPlayTrigger);

    TriggerInfo onDeath;
    onDeath.Name = "OnDeath";
    onDeath.Description = "When this card dies.";
    onDeath.Event = "card_died";

    auto onDeathTrigger = std::make_shared<Trigger>("card_died");

    registry.RegisterShared("on_death", onDeathTrigger);
    //registry.RegisterShared(2, onDeathTrigger);

    TriggerInfo onTurnStart;
    onTurnStart.Name = "OnTurnStart";
    onTurnStart.Description = "When turn starts.";
    onTurnStart.Event = "turn_started";

    auto onTurnStartTrigger = std::make_shared<Trigger>("turn_started");

    registry.RegisterShared("on_turn_start", onTurnStartTrigger);

    TriggerInfo onTurnEnd;
    onTurnEnd.Name = "OnTurnEnd";
    onTurnEnd.Description = "When turn Ends.";
    onTurnEnd.Event = "turn_ended";

    auto onTurnEndTrigger = std::make_shared<Trigger>("turn_ended");

    registry.RegisterShared("on_turn_end", onTurnEndTrigger);

}



/*

enum BuiltInTriggerId {
    TRIGGER_ON_PLAY = 1,
    TRIGGER_ON_DEATH = 2
};

registry.RegisterShared(TRIGGER_ON_PLAY, onPlayTrigger);

*/