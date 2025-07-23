
#pragma once
#include <functional>
#include <unordered_map>
#include <vector>
#include <string>
#include <memory>

class Event;

class EventBus {
public:
    using HandlerFunc = std::function<void(const std::shared_ptr<Event>&)>;

    void Subscribe(const std::string& eventType, HandlerFunc handler);
    void Emit(const std::shared_ptr<Event>& event);

private:
    std::unordered_map<std::string, std::vector<HandlerFunc>> Listeners;
};
