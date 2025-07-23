#include "TriggerObserver.h"
#include <iostream>

void TriggerObserver::Subscribe(const std::string& eventName, TriggerCallback callback) {
    Listeners[eventName].push_back(callback);
}

void TriggerObserver::Emit(const std::string& eventName, const std::unordered_map<std::string, std::string>& data) {
    auto it = Listeners.find(eventName);
    if (it != Listeners.end()) {
        for (const auto& callback : it->second) {
            callback(data);
        }
    }
}

void TriggerObserver::Unsubscribe(const std::string& eventName, TriggerCallback callback) {
    // maybe later...
}

void TriggerObserver::Clear() {
    Listeners.clear();
}

void TriggerObserver::PrintListeners(const std::string& eventName) const {
    auto it = Listeners.find(eventName);
    if (it != Listeners.end()) {
        std::cout << "🔔 " << it->second.size() << " listeners for '" << eventName << "'\n";
    }
}
