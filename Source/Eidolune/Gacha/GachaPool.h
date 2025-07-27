#pragma once
#include <string>
#include <vector>

class GachaPool {
public:
    std::string Name;
    std::string BannerType;
    int PityThreshold;
    std::vector<std::string> FeaturedIds;
};