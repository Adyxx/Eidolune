#include "UserRegistry.h"

UserRegistry& UserRegistry::Instance() {
    static UserRegistry instance;
    return instance;
}

void UserRegistry::Register(int id, const std::shared_ptr<User>& user) {
    Users[id] = user;
}

std::shared_ptr<User> UserRegistry::Get(int id) const {
    auto it = Users.find(id);
    return it != Users.end() ? it->second : nullptr;
}
