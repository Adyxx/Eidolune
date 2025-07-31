#pragma once

#include <unordered_map>
#include <memory>
#include "../Core/Subtype.h"

class SubtypeRegistry {
public:
    static SubtypeRegistry& Instance();

    void Register(int id, std::shared_ptr<Subtype> subtype);
    std::shared_ptr<Subtype> Get(int id) const;
    void Clear();

private:
    std::unordered_map<int, std::shared_ptr<Subtype>> registry;

    SubtypeRegistry() = default;
    SubtypeRegistry(const SubtypeRegistry&) = delete;
    SubtypeRegistry& operator=(const SubtypeRegistry&) = delete;
};
