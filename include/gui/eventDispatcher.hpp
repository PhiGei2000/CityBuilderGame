#pragma once
#include <functional>

template<typename Event>
class EventDispatcher {
  public:
    using EventDelegate = std::function<void(Event&)>;

  private:
    std::vector<EventDelegate> subscribers;

  public:
    inline void subscribe(EventDelegate&& delegate) {
        this->subscribers.push_back(std::forward<EventDelegate>(delegate));
    }

    inline EventDispatcher& operator+=(EventDelegate&& delegate) {
        subscribers.push_back(std::forward<EventDelegate>(delegate));

        return *this;
    }

    inline void invoke(Event& event) const {
        for (const EventDelegate& delegate : subscribers) {
            delegate.operator()(event);
        }
    }
};
