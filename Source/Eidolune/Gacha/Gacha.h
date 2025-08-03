#pragma once
#include "User.h"
#include "UserData.h"

namespace Gacha {
    void Run(std::shared_ptr<User> user, UserData& userData);
}
