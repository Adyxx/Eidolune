#pragma once

#include <memory>
#include "User.h"
#include "UserData.h"

namespace UserDataLoader {
    UserData LoadAllForUser(std::shared_ptr<User> user);
    void ApplyUserCharacterDataToCharacters(UserData& userData);
}
