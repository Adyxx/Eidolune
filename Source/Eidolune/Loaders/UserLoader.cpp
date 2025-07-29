#include "UserLoader.h"
#include "../Registry/UserRegistry.h"
#include "../Http/ApiClient.h"
#include <iostream>

std::unordered_map<int, std::shared_ptr<User>> UserLoader::LoadAll() {
    std::cout << "🔄 Loading Users from API...\n";
    std::unordered_map<int, std::shared_ptr<User>> result;

    json usersJson = ApiClient::Get("users/");

    for (const auto& u : usersJson) {
        if (!u.contains("username") || !u.contains("id")) {
            std::cerr << "User JSON missing username or id: " << u.dump() << "\n";
            continue;
        }

        auto user = std::make_shared<User>(
            u["id"].get<int>(),
            u["username"].get<std::string>(), 
            u.value("rank", 0)
        );

        int id = u["id"].get<int>();
        result[id] = user;
        UserRegistry::Instance().Register(id, user);
    }

    return result;
}
