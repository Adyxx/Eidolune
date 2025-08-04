#pragma once
#include <nlohmann/json.hpp>

struct GachaPullTracker {
    int TotalPulls = 0;
    int PullsSinceLastRare = 0;
    bool NextLegendaryGuaranteedFeatured = false;

    nlohmann::json ToJson() const;
    static GachaPullTracker FromJson(const nlohmann::json& j);
};
