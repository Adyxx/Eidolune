#pragma once
#include <unordered_map>
#include <memory>

class Subtype;

class SubtypeLoader {
public:
    static std::unordered_map<int, std::shared_ptr<Subtype>> LoadAll();
};
