cmake_minimum_required(VERSION 3.8)
project(auth)


INCLUDE_DIRECTORIES(/usr/local/include)
link_directories(/usr/local/lib)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        User.h
        Session.h
        Session.cpp
        User.cpp
        Auth1.cpp)

add_executable(auth ${SOURCE_FILES})

target_link_libraries(auth wthttp wt boost_system wtdbo wtdbosqlite3)