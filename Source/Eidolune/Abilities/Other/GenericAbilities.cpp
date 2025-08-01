
#include "GenericAbilities.h"
#include "../Registry/CharacterAbilityRegistry.h"
#include "Target.h"
#include "EffectDefinitions.h"
#include "Character.h"

void RegisterGenericAbilities() {
    CharacterAbilityRegistry::Instance().Register("draw_1", {
        [](Character& self, GameState& state) {
            for (auto& player : state.Players) {
                auto mainChar = player->DeckRef->MainCharacter.get();
                if (mainChar && mainChar == &self) {
                    std::cout << "📘 " << self.Name << " triggers passive: draw 1 card\n";
                    DrawCard(nullptr, Target::FromPlayer(player.get()), 1);
                }
            }
        },
        "Draw 1 card."
    });

    CharacterAbilityRegistry::Instance().Register("gain_2_energy", {
        [](Character& self, GameState& state) {
            for (auto& player : state.Players) {
                if (player->DeckRef->MainCharacter.get() == &self) {
                    std::cout << "⚡ " << self.Name << " gains 2 energy!\n";
                    player->Energy += 2;
                }
            }
        },
        "Gain 2 energy."
    });
}



// Whenever you play a card of a *new* type this turn, gain +1 energy (max once per type per turn).
