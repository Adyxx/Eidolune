#pragma once
#include <nlohmann/json.hpp>

struct GachaPullTracker {
    int BannerId;
    int TotalPulls = 0;
    int PullsSinceLastRare = 0;

    nlohmann::json ToJson() const;
    static GachaPullTracker FromJson(const nlohmann::json& j);
};
