#include "EffectHelpers.h"
#include <iostream>
#include <algorithm>
#include "Subtype.h"

namespace EffectHelpers {

bool MoveCardToZone(GameCard* rawCard, Player* player,
                    std::vector<std::shared_ptr<GameCard>>& fromZone,
                    std::vector<std::shared_ptr<GameCard>>& toZone,
                    const std::string& fromName,
                    const std::string& toName) {
    auto it = std::find_if(fromZone.begin(), fromZone.end(),
        [&](const std::shared_ptr<GameCard>& ptr) { return ptr.get() == rawCard; });

    if (it != fromZone.end()) {
        std::cout << "📤 Moving " << rawCard->GetName() << " from " << fromName << " to " << toName << ".\n";
        toZone.push_back(*it);
        fromZone.erase(it);
        return true;
    }
    return false;
}


int PromptChoice(Player* player, const std::vector<std::string>& options) {
    if (options.size() == 1) return 0;
    return player->PromptChooseOption(options);
}

} 
