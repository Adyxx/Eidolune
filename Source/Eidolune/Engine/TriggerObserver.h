#pragma once
#include <unordered_map>
#include <vector>
#include <string>
#include <functional>
#include <optional>

using TriggerCallback = std::function<void(const std::unordered_map<std::string, void*>&)>;

struct EffectHandle {
    int id;
    std::string eventName;
    TriggerCallback callback;
    void* owner = nullptr;
    bool active = true;
};

class TriggerObserver {
public:
    int Subscribe(const std::string& eventName, TriggerCallback callback, void* owner = nullptr);
    void Emit(const std::string& eventName, const std::unordered_map<std::string, void*>& data);

    void UnsubscribeById(int id);
    void UnsubscribeByOwner(void* owner, bool permanent = true);
    void SetOwnerActive(void* owner, bool active);

    void Clear();
    void PrintListeners(const std::string& eventName) const;

private:
    std::unordered_map<std::string, std::vector<EffectHandle>> Listeners;
    int NextId = 1;
};
