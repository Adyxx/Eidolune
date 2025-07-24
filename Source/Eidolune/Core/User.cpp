

#include "User.h"

User::User(const std::string& name, int rank) : Username(name), Rank(rank) {}

std::string User::ToString() const { return Username; }