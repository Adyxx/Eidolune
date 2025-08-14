#include "TriggerRegistry.h"
#include "TriggerDefinitions.h"

void RegisterTriggerDefinitions() {
    auto& registry = TriggerRegistry::Instance();
    /*
    TriggerInfo OnDeckBuild;
    OnDeckBuild.Name = "OnDeckBuild";
    OnDeckBuild.Description = "On deck build alteration for card-max-copy-per-deck restrictions.";
    OnDeckBuild.Event = "";

    auto OnDeckBuildTrigger = std::make_shared<Trigger>("");
    registry.RegisterShared("on_deck_build", OnDeckBuildTrigger);
    */

    TriggerInfo onPlay;
    onPlay.Name = "OnPlay";
    onPlay.Description = "When this card is played.";
    onPlay.Event = "card_played";
    auto onPlayTrigger = std::make_shared<Trigger>("card_played");
    registry.RegisterShared("on_play", onPlayTrigger);

    TriggerInfo onDeath;
    onDeath.Name = "OnDeath";
    onDeath.Description = "When this card dies.";
    onDeath.Event = "card_died";
    auto onDeathTrigger = std::make_shared<Trigger>("card_died");
    registry.RegisterShared("on_death", onDeathTrigger);

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

    TriggerInfo onAttack;
    onAttack.Name = "OnAttack";
    onAttack.Description = "When unit attacks.";
    onAttack.Event = "card_attacked";
    auto onAttackTrigger = std::make_shared<Trigger>("card_attacked");
    registry.RegisterShared("on_attack", onAttackTrigger);

    // ******************************************************

    TriggerInfo onDamageReceived;
    onDamageReceived.Name = "onDamageReceived";
    onDamageReceived.Description = "When card receives damage.";
    onDamageReceived.Event = "damage_received";
    auto onDamageReceivedTrigger = std::make_shared<Trigger>("damage_received");
    registry.RegisterShared("on_damage_received", onDamageReceivedTrigger);

    TriggerInfo onDamageDealt;
    onDamageDealt.Name = "onDamageDealt";
    onDamageDealt.Description = "When card deals damage.";
    onDamageDealt.Event = "damage_dealt";
    auto onDamageDealtTrigger = std::make_shared<Trigger>("damage_dealt");
    registry.RegisterShared("on_damage_dealt", onDamageDealtTrigger);

    TriggerInfo onCardDrawn;
    onCardDrawn.Name = "onCardDrawn";
    onCardDrawn.Description = "When card is drawn.";
    onCardDrawn.Event = "card_drawn";
    auto onCardDrawnTrigger = std::make_shared<Trigger>("card_drawn");
    registry.RegisterShared("on_card_draw", onCardDrawnTrigger);



    //////////////////////////////////////////////////////////////

    TriggerInfo time_1_opt1;
    time_1_opt1.Name = "time_1_opt1";
    time_1_opt1.Description = "";
    time_1_opt1.Event = "time_1_opt1";
    auto time_1_opt1Trigger = std::make_shared<Trigger>("time_1_opt1");
    registry.RegisterShared("time_1_opt1", time_1_opt1Trigger);

    TriggerInfo time_1_opt2;
    time_1_opt2.Name = "time_1_opt2";
    time_1_opt2.Description = "";
    time_1_opt2.Event = "time_1_opt2";
    auto time_1_opt2Trigger = std::make_shared<Trigger>("time_1_opt2");
    registry.RegisterShared("time_1_opt2", time_1_opt2Trigger);

    TriggerInfo time_2_opt1;
    time_2_opt1.Name = "time_2_opt1";
    time_2_opt1.Description = "";
    time_2_opt1.Event = "time_2_opt1";
    auto time_2_opt1Trigger = std::make_shared<Trigger>("time_2_opt1");
    registry.RegisterShared("time_2_opt1", time_2_opt1Trigger);

    TriggerInfo time_2_opt2;
    time_2_opt2.Name = "time_2_opt2";
    time_2_opt2.Description = "";
    time_2_opt2.Event = "time_2_opt1";
    auto time_2_opt2Trigger = std::make_shared<Trigger>("time_2_opt1");
    registry.RegisterShared("time_2_opt2", time_2_opt2Trigger);

    TriggerInfo time_3_opt1;
    time_3_opt1.Name = "time_3_opt1";
    time_3_opt1.Description = "";
    time_3_opt1.Event = "time_3_opt1";
    auto time_3_opt1Trigger = std::make_shared<Trigger>("time_3_opt1");
    registry.RegisterShared("time_3_opt1", time_3_opt1Trigger);

    TriggerInfo time_3_opt2;
    time_3_opt2.Name = "time_3_opt2";
    time_3_opt2.Description = "";
    time_3_opt2.Event = "time_3_opt2";
    auto time_3_opt2Trigger = std::make_shared<Trigger>("time_3_opt2");
    registry.RegisterShared("time_3_opt2", time_3_opt2Trigger);



}



