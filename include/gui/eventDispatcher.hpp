#pragma once
#include <functional>

template<typename Event>
class EventDispatcher {
    public:
        typedef std::function<void(const Event&)> EventDelegate;
    
    private:
        std::vector<EventDelegate> subscribers;

    public:
        void subscribe(EventDelegate&& function);

        EventDispatcher& operator +=(EventDelegate&& delegate);

        void invoke(const Event& event) const;
};
