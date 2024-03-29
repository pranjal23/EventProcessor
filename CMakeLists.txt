cmake_minimum_required(VERSION 3.4)
project(TestEvent)
SET( CMAKE_AUTOMOC ON )
SET( CMAKE_INCLUDE_CURRENT_DIR ON )
SET(Boost_USE_STATIC_LIBS OFF)
SET(CMAKE_CXX_STANDARD 14)
SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/cmake)

cmake_policy(SET CMP0015 NEW)
FILE(GLOB SRC "*.cpp")
FILE(GLOB INC_FILES "*.h")

FIND_PACKAGE( Boost 1.63 REQUIRED )

ADD_SUBDIRECTORY(EventProcessor)

INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIRS} )

ADD_EXECUTABLE( TestEvent ${SRC} ${INC_FILES})

SET(BASIC_LIBS
    boost_system
    boost_chrono
    boost_thread
    boost_filesystem
    pthread
    EventProcessor)


TARGET_LINK_LIBRARIES( TestEvent
    ${BASIC_LIBS}
)

