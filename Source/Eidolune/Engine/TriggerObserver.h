#pragma once
#include <functional>
#include <unordered_map>
#include <vector>
#include <string>
#include <memory>

class TriggerObserver {
public:
    using TriggerCallback = std::function<void(const std::unordered_map<std::string, std::string>&)>;

    void Subscribe(const std::string& eventName, TriggerCallback callback);
    void Emit(const std::string& eventName, const std::unordered_map<std::string, std::string>& data = {});
    void Unsubscribe(const std::string& eventName, TriggerCallback callback);
    void Clear();
    void PrintListeners(const std::string& eventName) const;

private:
    std::unordered_map<std::string, std::vector<TriggerCallback>> Listeners;
};
