#pragma once
#include <string>
#include <optional>
#include "Types.h"

class Trigger {
public:
    std::string ScriptReference;
    std::string Description;

    Trigger(const std::string& ref, const std::string& desc = "");

    std::string ToString() const;
};
