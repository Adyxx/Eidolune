
#pragma once
#include <string>

class User {
public:
    int Id;
    std::string Username;
    int Rank;

    User(int id, const std::string& name, int rank = 0);

    std::string ToString() const;
};
