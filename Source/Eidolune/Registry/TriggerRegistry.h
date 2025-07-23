#pragma once
#include <string>
#include <unordered_map>
#include <functional>

struct TriggerInfo {
    std::string Description;
    std::string Event;
    std::function<void()> Builder;
    std::string Name;
};

class TriggerRegistry {
public:
    static void Init();
    static void Register(const std::string& code, const TriggerInfo& info);
    static const std::unordered_map<std::string, TriggerInfo>& GetAll();
    static const TriggerInfo* Get(const std::string& code);

private:
    static std::unordered_map<std::string, TriggerInfo> Registry;
};
