
#pragma once
#include <string>

class User {
public:
    std::string Username;
    int Rank;

    User(const std::string& name, int rank = 0) : Username(name), Rank(rank) {}

    std::string ToString() const { return Username; }
};
