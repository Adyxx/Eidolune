#pragma once
#include <unordered_map>
#include <memory>
#include <string>
#include "../Registry/TriggerRegistry.h"

class Trigger;

class TriggerLoader {
public:
    static std::unordered_map<std::string, std::shared_ptr<Trigger>> LoadAll();
};
