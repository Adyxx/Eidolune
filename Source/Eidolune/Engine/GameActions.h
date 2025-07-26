#pragma once

#include <memory>
#include "Player.h"
#include "GameCard.h"
#include "TriggerObserver.h"
#include <memory>

struct Target {
    enum class Type { NONE, CARD, PLAYER } type = Type::NONE;
    void* ptr = nullptr;

    static Target FromCard(GameCard* card) {
        return Target{Type::CARD, card};
    }
    static Target FromPlayer(Player* player) {
        return Target{Type::PLAYER, player};
    }
};

namespace GameActions {

    void PlayCard(Player* player, int index, std::shared_ptr<TriggerObserver> observer);
    void Attack(Player* attacker, Player* defender);
    void UseAbility(Player* player, Player* opponent);
    void StartTurn(Player* player);
    void EndTurn(Player* player);


    //void ResolveCombat(std::shared_ptr<GameCard> attackerCard, std::shared_ptr<GameCard> targetCard, Player* attacker, Player* defender);
    void ResolveCombat(std::shared_ptr<GameCard> attackerCard, GameCard* targetCard, Player* attacker, Player* defender);
    //void ResolveDamage(std::shared_ptr<GameCard> source, GameCard* targetCard, int amount, const std::string& type);
    //void ResolveDamage(std::shared_ptr<GameCard> source, Player* playerTarget, int amount, const std::string& type);
    void ResolveDamage(GameCard* source, GameCard* targetCard, int amount, const std::string& type);
    void ResolveDamage(GameCard* source, Player* playerTarget, int amount, const std::string& type);

    
    void ResolveHeal(std::shared_ptr<GameCard> source, void* target, int amount);

    void ShowHand(Player* player);
    void ShowBoard(Player* player);
    void ShowPlayerState(Player* player);

    //std::vector<void*> GetTargets(Player* player, Player* opponent, TargetSpec spec);
    std::vector<Target> GetTargets(Player* player, Player* opponent, TargetSpec spec);
    //void* ChooseTarget(Player* player, const std::vector<void*>& targets);
    Target ChooseTarget(Player* player, const std::vector<Target>& targets);
}
