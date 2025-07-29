#pragma once
#include <unordered_map>
#include <string>
#include <memory>
#include "../Core/Subtype.h"

class SubtypeRegistry {
public:
    static SubtypeRegistry& Instance();

    void Register(const std::string& id, std::shared_ptr<Subtype> subtype);
    std::shared_ptr<Subtype> Get(const std::string& id) const;
    void Clear();

private:
    std::unordered_map<std::string, std::shared_ptr<Subtype>> subtypes;
};
