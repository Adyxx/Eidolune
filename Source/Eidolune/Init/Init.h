#pragma once

#include "User.h"
#include <memory>
#include "UserData.h"

namespace EidoluneInit {
    void RegisterAll();
    void LoadAll();
    UserData LoadForUser(std::shared_ptr<User> user);
}
