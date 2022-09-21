#pragma once
#include <functional>

template<typename Event>
class EventDispatcher {
  public:
    using EventDelegate = std::function<void(Event&)>;

  private:
    std::vector<EventDelegate> subscribers;

  public:
    void subscribe(EventDelegate&& function);

    EventDispatcher& operator+=(EventDelegate&& delegate);

    void invoke(Event& event) const;
};
