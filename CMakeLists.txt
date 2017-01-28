cmake_minimum_required(VERSION 3.6)
project(ListenUsb)

set(CMAKE_CXX_STANDARD 11)

find_package(Boost COMPONENTS system filesystem REQUIRED)
if(Boost_FOUND)
    set(SOURCE_FILES main.cpp)
    add_executable(ListenUsb ${SOURCE_FILES})
    target_link_libraries(ListenUsb ${Boost_LIBRARIES})
    include_directories(${Boost_INCLUDE_DIRS})
endif()


if(WIN32)
    target_link_libraries(ListenUsb wsock32 ws2_32)
endif()