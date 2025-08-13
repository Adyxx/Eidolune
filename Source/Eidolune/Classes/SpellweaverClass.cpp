#include "SpellweaverClass.h"
#include <iostream>
#include "CardRegistry.h"
#include "CardUtils.h"
#include "EffectHelpers.h"
#include "TriggerObserver.h"
#include "TriggerRegistry.h"
#include "TriggerBuilder.h"
#include "ConditionEvaluator.h"
#include "Effect.h"

SpellweaverClass::SpellweaverClass() {
    Name = "Spellweaver";
    Description = "Masters of combining magical fragments into powerful, unique spells.";
}

void SpellweaverClass::OnTurnStart(Player* player) {
    auto allFragments = CardRegistry::GetCardsByAuxType(AuxiliaryCardType::FRAGMENT);

    std::vector<std::shared_ptr<Card>> available;
    for (const auto& card : allFragments) {
        if (!player->HasChosenAuxCard(card)) {
            available.push_back(card);
        }
    }

    if (available.empty()) {
        std::cout << "[Spellweaver] No available fragment cards to offer.\n";
        return;
    }

    auto options = CardUtils::SelectRandomOptions(available, 3);
    player->PromptAuxCardChoice(AuxiliaryCardType::FRAGMENT, options);

    std::cout << "[Spellweaver] Offered " << options.size() << " fragment options.\n";

    TryMergeFragments(player);
}

// Subscribe just one binding (avoid re-subscribing everything)
static void SubscribeOneBinding(
    const std::shared_ptr<GameCard>& card,
    const std::shared_ptr<CardEffectBinding>& originalBinding,
    const std::shared_ptr<TriggerObserver>& observer)
{
    if (!originalBinding) { std::cout << "❌ Null binding passed to SubscribeOneBinding\n"; return; }

    // Work on a copy so we don't mutate original
    auto binding = std::make_shared<CardEffectBinding>(*originalBinding);

    auto triggerPtr = binding->GetTrigger();
    if (!triggerPtr) { std::cout << "⚠️ Skipping binding: no trigger found.\n"; return; }

    auto* meta = TriggerRegistry::Instance().GetInfo(triggerPtr->ScriptReference);
    if (!meta) { std::cout << "❌ No trigger metadata for " << triggerPtr->ScriptReference << "\n"; return; }

    binding->EventGameCard = card;            // bind to THIS runtime instance
    binding->ParentCard    = card->Model;     // make binding "belong" to the base model (nice for ToString/conditions)

    auto builder = TriggerBuilder::Build(binding);

    std::function<void(const std::unordered_map<std::string, void*>)> effectToRegister;
    if (auto conditionPtr = binding->GetCondition()) {
        std::string conditionCode = conditionPtr->ToString();
        int condVal = binding->GetConditionValue().value_or(0);
        effectToRegister = [=](const std::unordered_map<std::string, void*>& ctx) {
            if (ConditionEvaluator::Evaluate(conditionCode, card, condVal)) {
                builder(ctx);
            }
        };
    } else {
        effectToRegister = builder;
    }

    observer->Subscribe(meta->Event, effectToRegister);
}

void SpellweaverClass::TryMergeFragments(Player* player) {
    // 1) Collect all fragment GameCards in hand
    std::vector<std::shared_ptr<GameCard>> fragments;
    for (auto& gc : player->Hand) {
        if (gc->Model && gc->Model->AuxiliaryType == AuxiliaryCardType::FRAGMENT) {
            fragments.push_back(gc);
        }
    }

    if (fragments.size() < 2) {
        std::cout << "[Spellweaver] Not enough fragments to merge.\n";
        return;
    }

    // 2) Find first unmerged fragment to use as the base
    auto baseIt = std::find_if(fragments.begin(), fragments.end(),
                               [](const std::shared_ptr<GameCard>& gc) {
                                   return !gc->HasStatus(CardStatus::MERGED);
                               });

    if (baseIt == fragments.end()) {
        std::cout << "[Spellweaver] All fragments in hand are already merged; no new merge possible.\n";
        return;
    }

    auto base = *baseIt;
    std::cout << "[Spellweaver] Base fragment: " << base->GetName() << "\n";

    // 3) Gather donor fragments (exclude the chosen base)
    std::vector<std::shared_ptr<GameCard>> donors;
    for (auto& frag : fragments) {
        if (frag != base) {
            donors.push_back(frag);
        }
    }

    if (donors.empty()) {
        std::cout << "[Spellweaver] No donor fragments available for merge.\n";
        return;
    }

    // Track cost sum
    int totalBaseCost = base->Model->Cost;
    totalBaseCost += donors.front()->Model->Cost; // only one donor for this merge

    // 4) Copy bindings from the first donor
    auto donor = donors.front();
    std::cout << "  + Donor fragment: " << donor->GetName()
              << " (" << donor->Model->EffectBindings.size() << " bindings)\n";

    size_t added = 0;
    for (auto& binding : donor->Model->EffectBindings) {
        if (!binding || !binding->BoundEffect) continue;

        if (binding->BoundEffect->Name == "MergeFragments") {
            std::cout << "    - Skipping MergeFragments effect from donor.\n";
            continue;
        }

        auto runtimeCopy = std::make_shared<CardEffectBinding>(*binding);
        runtimeCopy->ParentCard = base->Model;

        base->RuntimeEffectBindings.push_back(runtimeCopy);
        SubscribeOneBinding(base, runtimeCopy, player->Observer);
        ++added;

        std::cout << "    - Added runtime effect: " << binding->ToString() << "\n";
    }

    // 5) Exile the donor
    std::cout << "  ⟶ Exiling donor: " << donor->GetName() << "\n";
    EffectHelpers::MoveCardToZone(donor.get(), player, player->Hand, player->ExileZone, "Hand", "Exile");

    // 6) Apply cost adjustment
    int delta = totalBaseCost - base->Model->Cost;
    if (delta != 0) {
        // use a stable ID for this merge adjustment so it can be replaced/removed if needed
        constexpr int MERGE_COST_MODIFIER_ID = 9999; 
        base->ApplyCostModifier(MERGE_COST_MODIFIER_ID, delta);
        std::cout << "[Spellweaver] Adjusted cost by " << delta 
                  << ". New cost: " << base->CurrentCost() << "\n";
    }

    // 7) Mark base as merged
    std::cout << "[Spellweaver] Merge complete. Base '" << base->GetName()
              << "' gained " << added << " runtime binding(s). Now has "
              << base->RuntimeEffectBindings.size() << " runtime binding(s).\n";

    base->AddStatus(CardStatus::MERGED);
}

