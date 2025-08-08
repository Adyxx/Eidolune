#pragma once
#include <memory>
#include <vector>
#include <string>
#include <optional>
#include "GameCard.h"
#include "Player.h"

namespace EffectHelpers {

bool MoveCardToZone(GameCard* card,
                    Player* player,
                    std::vector<std::shared_ptr<GameCard>>& fromZone,
                    std::vector<std::shared_ptr<GameCard>>& toZone,
                    const std::string& fromName,
                    const std::string& toName);


int PromptChoice(Player* player, const std::vector<std::string>& options);

}