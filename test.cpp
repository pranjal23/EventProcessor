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
#include <boost/test/included/unit_test.hpp>
#include <boost/test/test_case_template.hpp>
#include <chrono>
#include <EventProcessor/EventLib.hpp>

using namespace boost::unit_test;
using namespace std::this_thread;
using namespace std::chrono;

int64_t start_time = 0;

class RelativeTestEvent : public BaseEvent {
public:
    explicit RelativeTestEvent(int64_t timeFromStart, int64_t timeAtStart) : BaseEvent(timeFromStart, timeAtStart) {}

    void functionToExecute() override {
        std::cout << "Standard function triggered by on event! ";
    }
};

class TestEvent : public BaseEvent {
public:
    explicit TestEvent(int64_t timeFromNow):BaseEvent(timeFromNow) {}


    void functionToExecute() override {
        std::cout << "Standard function triggered by on event! ";
    }

    void printTestEventDetails()
    {
        std::cout << "Extened function!" << std::endl;
    }
};


class Test : EventSource {
public:
    EventManager* eventManager;

    Test() : EventSource()
    {
        eventManager = EventManager::instance();
        eventManager->addObserver(this);
        passed = false;

        std::cout << "Adding relative test event..." << std::endl;
        BaseEvent* o_event = new RelativeTestEvent(2000, start_time + 1000);
        o_event->id = "RELATIVE_TEST_EVENT";
        addEvent(eventManager, o_event);

        std::cout << "Adding test event..." << std::endl;
        BaseEvent* event = new TestEvent(4000);
        event->id = "TEST_EVENT";
        event->reccuring = true;
        addEvent(eventManager, event);
    }


    void updateEvent(BaseEvent* e) override
    {
        e->functionToExecute();

        boost::chrono::milliseconds ms =
                boost::chrono::duration_cast<boost::chrono::milliseconds> (boost::chrono::high_resolution_clock::now().time_since_epoch());
        int64_t triggered_afer_time = ms.count() - start_time;

        BOOST_ASSERT( triggered_afer_time >= e->wait_time_in_millisecs );
        passed = true;

        // Call other extended features of TestEvent...
        TestEvent* te = dynamic_cast<TestEvent*>(e);
        if (te) {
            te->printTestEventDetails();
        }
    }

    bool passed;

};

void testEventScheduling() {
    boost::chrono::milliseconds ms =
            boost::chrono::duration_cast<boost::chrono::milliseconds> (boost::chrono::high_resolution_clock::now().time_since_epoch());
    start_time = ms.count();

    Test test;
    sleep_for(std::chrono::seconds(15));
    BOOST_CHECK_EQUAL(test.passed,true);
}

test_suite*
init_unit_test_suite( int argc, char* argv[] )
{
    framework::master_test_suite().add( BOOST_TEST_CASE( &testEventScheduling ) );

    return 0;
}
