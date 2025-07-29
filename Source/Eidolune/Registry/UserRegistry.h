#pragma once
#include <unordered_map>
#include <memory>
#include <string>
#include "../Core/User.h"

class UserRegistry {
public:
    static UserRegistry& Instance();

    void Register(int id, const std::shared_ptr<User>& user);
    std::shared_ptr<User> Get(int id) const;

    const std::unordered_map<int, std::shared_ptr<User>>& GetAll() const {
        return Users;
    }

private:
    std::unordered_map<int, std::shared_ptr<User>> Users;

    UserRegistry() = default;
    UserRegistry(const UserRegistry&) = delete;
    UserRegistry& operator=(const UserRegistry&) = delete;
};
