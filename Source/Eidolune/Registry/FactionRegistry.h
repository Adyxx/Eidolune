#pragma once

#include <unordered_map>
#include <memory>
#include "../Core/Faction.h"

class FactionRegistry {
public:
    static FactionRegistry& Instance();

    void Register(int id, std::shared_ptr<Faction> faction);
    std::shared_ptr<Faction> Get(int id) const;
    void Clear();

    const std::unordered_map<int, std::shared_ptr<Faction>>& GetAll() const {
        return factions;
    }

private:
    std::unordered_map<int, std::shared_ptr<Faction>> factions;

    FactionRegistry() = default;
    FactionRegistry(const FactionRegistry&) = delete;
    FactionRegistry& operator=(const FactionRegistry&) = delete;
};
