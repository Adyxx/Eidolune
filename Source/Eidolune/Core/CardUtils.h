#pragma once
#include <vector>
#include <random>

namespace CardUtils {
    template<typename T>
    std::vector<T> SelectRandomOptions(const std::vector<T>& input, int count) {
        std::vector<T> shuffled = input;
        std::shuffle(shuffled.begin(), shuffled.end(), std::mt19937(std::random_device()()));

        if (count >= shuffled.size()) return shuffled;
        return std::vector<T>(shuffled.begin(), shuffled.begin() + count);
    }
}


/*


usage

auto chosen = CardUtils::SelectRandomOptions(oaths, 2);

*/