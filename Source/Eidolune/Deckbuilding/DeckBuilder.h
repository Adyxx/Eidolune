#pragma once
#include <memory>
#include "User.h"
#include "UserData.h"
#include "Deck.h"

namespace DeckBuilder {

    void Run(std::shared_ptr<User> currentUser, UserData& currentUserData);

}
