

#pragma once
#include <memory>
#include "../Data/UserData.h"
#include "../Core/User.h"

class UserDataUploader {
public:
    static void UploadUserData(std::shared_ptr<User> user, const UserData& data);
};
