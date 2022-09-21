#include "gui/eventDispatcher.hpp"

template<typename Event>
void EventDispatcher<Event>::subscribe(EventDelegate&& func) {
    subscribers.push_back(std::forward<EventDelegate>(func));
}

template<typename Event>
EventDispatcher<Event>& EventDispatcher<Event>::operator+=(EventDelegate&& delegate) {
    this->subscribers.push_back(std::forward<EventDelegate>(delegate));

    return *this;
}

template<typename Event>
void EventDispatcher<Event>::invoke(Event& event) const {
    for (const EventDelegate& delegate : subscribers) {
        delegate.operator()(event);
    }
}

#include "events/mouseEvents.hpp"

template void EventDispatcher<MouseMoveEvent>::subscribe(EventDispatcher<MouseMoveEvent>::EventDelegate&&);
template EventDispatcher<MouseMoveEvent>& EventDispatcher<MouseMoveEvent>::operator+=(EventDispatcher<MouseMoveEvent>::EventDelegate&&);
template void EventDispatcher<MouseMoveEvent>::invoke(MouseMoveEvent&) const;

template void EventDispatcher<MouseButtonEvent>::subscribe(EventDispatcher<MouseButtonEvent>::EventDelegate&&);
template EventDispatcher<MouseButtonEvent>& EventDispatcher<MouseButtonEvent>::operator+=(EventDispatcher<MouseButtonEvent>::EventDelegate&&);
template void EventDispatcher<MouseButtonEvent>::invoke(MouseButtonEvent&) const;
