
#pragma once
#include <vector>
#include <memory>

class Player;
class Card;
class Character;
class Event;

class GameState {
public:
    std::shared_ptr<Player> PlayerOne;
    std::shared_ptr<Player> PlayerTwo;

    std::vector<std::shared_ptr<Event>> EventLog;

    int TurnNumber = 1;
    bool IsPlayerOneTurn = true;

    GameState(std::shared_ptr<Player> p1, std::shared_ptr<Player> p2)
        : PlayerOne(p1), PlayerTwo(p2) {}

    void AdvanceTurn();
    void LogEvent(const std::shared_ptr<Event>& event);
};
