#pragma once
#include <vector>
#include <random>
#include <memory>
#include <functional>
#include <iostream>

#include "Types.h"
#include "GameCard.h"

#include "CardEffectBinding.h"
#include "ConditionEvaluator.h"
#include "TriggerObserver.h"
#include "TriggerBuilder.h"
#include "TriggerRegistry.h"


class Player;

namespace CardUtils {
    template<typename T>
    std::vector<T> SelectRandomOptions(const std::vector<T>& input, int count) {
        std::vector<T> shuffled = input;
        std::shuffle(shuffled.begin(), shuffled.end(), std::mt19937(std::random_device()()));

        if (count >= shuffled.size()) return shuffled;
        return std::vector<T>(shuffled.begin(), shuffled.begin() + count);
    }

    void SubscribeCardTriggers(std::shared_ptr<GameCard> card, std::shared_ptr<TriggerObserver> observer);

    std::shared_ptr<GameCard> RegisterCardMidGame(
        std::shared_ptr<Card> templateCard,
        Player* owner,
        CardZone zone,
        std::shared_ptr<TriggerObserver> observer
    );

}


/*
usage
auto chosen = CardUtils::SelectRandomOptions(oaths, 2);
*/