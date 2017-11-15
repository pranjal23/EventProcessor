//
// Created by pranjal on २०/९/१७.
//

#ifndef EVENTMANAGER_H
#define EVENTMANAGER_H
/* Copyright (c) 2017 Pranjal Swarup

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include <vector>
#include <queue>
#include <mutex>
#include <thread>
#include <boost/asio.hpp>
#include <boost/thread/thread.hpp>
#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

#include "BaseEvent.h"
#include "Observable.h"
#include "EventExecutor.h"

class EventSource;

class EventManager : Observable
{
public:
    static EventManager *instance()
    {
        if (!theInstance)
            theInstance = new EventManager;
        return theInstance;
    }

    virtual ~EventManager() override
    {

    }

    void addObserver(Observer* o) override
    {
        observers.push_back(o);
    }

    void updateObservers(BaseEvent* e) override
    {
        for(std::vector<Observer*>::iterator itr = observers.begin(); itr!=observers.end(); ++itr)
        {
            if(*itr)
                (*itr)->updateEvent(e);
        }
    }

    void scheduleEvent()
    {
        BaseEvent* e = eventStack.front();
        eventStack.pop();
        boost::asio::io_service io;
        EventExecutor eventExecutor(e, static_cast<void*>(this), io);
        boost::thread t(boost::bind(&boost::asio::io_service::run, &io));
        io.run();
        t.detach();
    }

    void push_Event(EventSource* source, BaseEvent* e)
    {
        assert(source != nullptr);
        push_Event(e);
    }

    void push_Event(EventExecutor* executor, BaseEvent* e)
    {
        assert(executor != nullptr);
        push_Event(e);
    }

private:
    EventManager(){}
    static EventManager* theInstance;
    std::vector<Observer*> observers;
    std::queue<BaseEvent*> eventStack;
    std::mutex pushlock_1;
    std::mutex pushlock_2;

    void push_Event(BaseEvent* e)
    {
        std::lock(pushlock_1, pushlock_2);
        std::lock_guard<std::mutex> lock1(pushlock_1, std::adopt_lock);
        std::lock_guard<std::mutex> lock2(pushlock_2, std::adopt_lock);

        eventStack.push(e);

        std::thread t{&EventManager::scheduleEvent, this};
        t.detach();
    }
};
#endif // EVENTMANAGER_H
