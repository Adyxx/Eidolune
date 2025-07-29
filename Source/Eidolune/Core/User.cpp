

#include "User.h"


User::User(int id, const std::string& name, int rank): Id(id), Username(name), Rank(rank) {}


std::string User::ToString() const { return Username + " (Rank: " + std::to_string(Rank) + ")"; }