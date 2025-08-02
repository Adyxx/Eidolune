#include "Init.h"

// Registry & Logic Setup
#include "../Abilities/ClassAbilities/BloodboundAbilities.h"
#include "../Abilities/ClassAbilities/GamblerAbilities.h"
#include "../Abilities/ClassAbilities/MachinistAbilities.h"
#include "../Abilities/ClassAbilities/OathboundAbilities.h"
#include "../Abilities/ClassAbilities/SpellweaverAbilities.h"
#include "../Abilities/ClassAbilities/SpecialistAbilities.h"
#include "../Abilities/ClassAbilities/SoulforgedAbilities.h"
#include "../Abilities/ClassAbilities/ShadeAbilities.h"

// ... include all class/generic/partner/solo
#include "../Abilities/Other/GenericAbilities.h"
#include "../Abilities/Other/PartnerAbilities.h"
#include "../Abilities/Other/SoloBonus.h"

#include "../Registry/DeckRestrictionRegistry.h"
#include "../Registry/CharacterAbilityRegistry.h"
#include "../Registry/CharacterClassRegistry.h"
#include "../Registry/DeckRestrictionRegistry.h"

// Definitions
#include "../Definitions/ConditionDefinitions.h"
#include "../Definitions/DeckRestrictionDefinitions.h"
#include "../Definitions/EffectDefinitions.h"
#include "../Definitions/TriggerDefinitions.h"


// Loaders
#include "../Loaders/TriggerLoader.h"
#include "../Loaders/EffectLoader.h"
#include "../Loaders/ConditionLoader.h"
#include "../Loaders/SubtypeLoader.h"
#include "../Loaders/CharacterLoader.h"
#include "../Loaders/CardLoader.h"
#include "../Loaders/CardEffectBindingLoader.h"
#include "../Loaders/UserLoader.h"
#include "../Loaders/DeckLoader.h"
#include "../Loaders/DeckCardLoader.h"
#include "../Loaders/UserDataLoader.h"

#include <iostream>

namespace EidoluneInit {

    void RegisterAll() {
        std::cout << "🔧 Registering all logic systems...\n";

        RegisterConditionFunctions();
        RegisterEffectFunctions();
        RegisterDeckRestrictions();

        RegisterGenericAbilities();
        RegisterPartnerAbilities();
        RegisterSoloBonus();

        RegisterBloodboundAbilities();
        RegisterGamblerAbilities();
        RegisterMachinistAbilities();
        RegisterOathboundAbilities();
        RegisterShadeAbilities();
        RegisterSoulforgedAbilities();
        RegisterSpecialistAbilities();
        RegisterSpellweaverAbilities();

        // ... Possibly future: RegisterMechanics, StatusEffects, etc.
    }

    void LoadAll() {
        std::cout << "📦 Loading general data from API...\n";

        TriggerLoader::LoadAll();
        EffectLoader::LoadAll();
        ConditionLoader::LoadAll();
        SubtypeLoader::LoadAll();
        CharacterLoader::LoadAll();
        CardLoader::LoadAll();
        CardEffectBindingLoader::LoadAll();
        UserLoader::LoadAll();
        //DeckLoader::LoadAll();
        //DeckCardLoader::LoadAll();
    }

    UserData LoadForUser(std::shared_ptr<User> user) {
        return UserDataLoader::LoadAllForUser(user);
    }

}
