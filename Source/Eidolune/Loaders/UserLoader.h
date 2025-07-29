#pragma once
#include <unordered_map>
#include <memory>
#include <string>
#include "../Core/User.h"

class UserLoader {
public:
    static std::unordered_map<int, std::shared_ptr<User>> LoadAll();
};
