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

#ifndef EVENT_H
#define EVENT_H

#include <boost/chrono.hpp>

using namespace boost::chrono;

class BaseEvent
{
public:
    explicit BaseEvent(int64_t exc_time_millisecs){
        this->wait_time_in_millisecs = exc_time_millisecs;
    }

    explicit BaseEvent(int64_t wait_in_millisecs, int64_t start_in_millisecs){
        boost::chrono::milliseconds ms =
                boost::chrono::duration_cast<boost::chrono::milliseconds> (boost::chrono::high_resolution_clock::now().time_since_epoch());
        int64_t curr_time = ms.count();
        this->wait_time_in_millisecs = (wait_in_millisecs + start_in_millisecs) - curr_time;
    }

    virtual ~BaseEvent() = default;

    int64_t wait_time_in_millisecs;
    bool reccuring;

    std::string id;
    virtual void functionToExecute()=0;
};

#endif // EVENT_H
