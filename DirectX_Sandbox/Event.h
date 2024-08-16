#pragma once
#include <functional>
#include <vector>

template<typename... Args>
class Event {
public:
    using Listener = std::function<void(Args...)>;

    void Subscribe(const Listener& listener) {
        listeners.push_back(listener);
    }

    void Notify(Args... args) {
        for (const auto& listener : listeners) {
            listener(args...);
        }
    }

private:
    std::vector<Listener> listeners;
};