#pragma once
#include <nlohmann/json.hpp>
#include <vector>
#include <string>
#include <unordered_map>
#include <memory>

class User;
class Character;

struct UserCharacterData {
    int CharacterId;
    std::shared_ptr<Character> ResolvedCharacter = nullptr;

    int Level = 1;

    std::unordered_map<std::string, std::string> EquippedItems;
    std::vector<int> Consumables;

    nlohmann::json ToJson() const;
    static UserCharacterData FromJson(const nlohmann::json& j);
    static UserCharacterData FromCharacter(Character* character);
};


/*

characterData.Sync(userData);
UserDataUploader::UploadUserData(user, userData);

*/