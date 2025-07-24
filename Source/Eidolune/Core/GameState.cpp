#include "GameState.h"

std::shared_ptr<Player> GameState::GetCurrentPlayer() {
    return Players[CurrentPlayer];
}

std::shared_ptr<Player> GameState::GetOpponent() {
    return Players[1 - CurrentPlayer];
}

void GameState::NextTurn() {
    CurrentPlayer = 1 - CurrentPlayer;
}
