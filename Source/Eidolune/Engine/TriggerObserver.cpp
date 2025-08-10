#include "TriggerObserver.h"
#include <iostream>

int TriggerObserver::Subscribe(const std::string& eventName, TriggerCallback callback, void* owner) {
    int id = NextId++;
    Listeners[eventName].push_back({id, eventName, std::move(callback), owner, true});
    return id;
}

void TriggerObserver::Emit(const std::string& eventName, const std::unordered_map<std::string, void*>& data) {
    auto it = Listeners.find(eventName);
    if (it == Listeners.end()) return;

    // Work on a copy so we can safely modify Listeners later
    auto callbacks = it->second;
    for (const auto& handle : callbacks) {
        if (handle.active) {
            handle.callback(data);
        }
    }
}

void TriggerObserver::UnsubscribeById(int id) {
    for (auto& [event, vec] : Listeners) {
        vec.erase(std::remove_if(vec.begin(), vec.end(),
            [id](const EffectHandle& h) { return h.id == id; }),
            vec.end());
    }
}

void TriggerObserver::UnsubscribeByOwner(void* owner, bool permanent) {
    for (auto& [event, vec] : Listeners) {
        if (permanent) {
            vec.erase(std::remove_if(vec.begin(), vec.end(),
                [owner](const EffectHandle& h) { return h.owner == owner; }),
                vec.end());
        } else {
            for (auto& h : vec) {
                if (h.owner == owner) {
                    h.active = false;
                }
            }
        }
    }
}

void TriggerObserver::SetOwnerActive(void* owner, bool active) {
    for (auto& [event, vec] : Listeners) {
        for (auto& h : vec) {
            if (h.owner == owner) {
                h.active = active;
            }
        }
    }
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
