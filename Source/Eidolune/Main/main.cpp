
#include "../Loaders/CardLoader.h"

#include "../Core/Effect.h"

#include "../Core/Trigger.h"
#include <iostream>

int main() {
    std::cout << "OK \n";
    
    auto cards = CardLoader::LoadAll("Content/Cards/cards.json");
    
    for (const auto& [id, card] : cards) {
        std::cout << "Loaded card: " << card->Name << " (Cost " << card->Cost << ")\n";
        for (const auto& effect : card->EffectBindings) {
            std::cout << "  Trigger: " << effect->BoundTrigger->ToString()
                      << ", Effect: " << effect->BoundEffect->ToString();
            if (effect->Value) std::cout << ", Value: " << *effect->Value;
            if (effect->Targeting) std::cout << ", Target: " << (int)*effect->Targeting;
            std::cout << "\n";
        }
    }
    
    return 0;
}
