
#include "UserDataUploader.h"
#include "../Http/ApiClient.h"
#include <iostream>

void UserDataUploader::UploadUserData(std::shared_ptr<User> user, const UserData& data) {
    if (!user) {
        std::cerr << "❌ No user logged in.\n";
        return;
    }

    UserData dataToUpload = data;
    dataToUpload.UserId = user->Id;

    json payload = dataToUpload.ToJson();

    ApiClient::Post("users/" + std::to_string(user->Id) + "/data/", payload);
    std::cout << "✅ Data uploaded for user " << user->Username << "\n";
    std::cout << "Uploading JSON:\n" << payload.dump(4) << "\n";
}