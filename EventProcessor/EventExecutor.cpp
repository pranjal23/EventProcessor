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


#include <iostream>
#include <boost/thread/thread.hpp>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

#include "EventExecutor.h"
#include "EventManager.h"

using namespace std::chrono;
EventManager* EventManager::theInstance = 0;
EventExecutor::EventExecutor(BaseEvent* event, void* em, boost::asio::io_service& io)
        : strand_(io),
          timer1_(io, boost::posix_time::seconds(0))
{
    this->event = event;
    this->eve_man = em;
    timer1_.expires_at(timer1_.expires_at() + boost::posix_time::seconds(0));
    timer1_.async_wait(strand_.wrap(boost::bind(&EventExecutor::processEvent, this)));
}

void EventExecutor::processEvent() {
    // Calculate the time when it needs to be executed...
    int64_t pause_in_secs = event->wait_time_in_millisecs / 1000;

    timer1_.expires_at(timer1_.expires_at() + boost::posix_time::seconds(pause_in_secs));
    timer1_.async_wait(strand_.wrap(boost::bind(&EventExecutor::triggerEvent, this)));
}

void EventExecutor::triggerEvent() {
    EventManager *eventManager = static_cast<EventManager *>(this->eve_man);
    eventManager->updateObservers(event);

    if ( event->reccuring )
        eventManager->push_Event(this, event);
}
