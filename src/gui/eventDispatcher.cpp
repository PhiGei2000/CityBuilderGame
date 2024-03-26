/*  Copyright (C) 2024  Philipp Geil <https://github.com/PhiGei2000>
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
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
