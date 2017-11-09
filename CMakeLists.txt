cmake_minimum_required(VERSION 3.8)
project(auth)


INCLUDE_DIRECTORIES(/usr/local/include)
link_directories(/usr/local/lib)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        src/User.h
        src/Session.h
        src/Session.cpp
        src/User.cpp
        src/Auth1.cpp src/AuthWidget.cpp src/AuthWidget.h src/Bridge.cpp src/Bridge.h src/UserDetailsModel.cpp src/UserDetailsModel.h src/RegistrationView.cpp src/RegistrationView.h)

add_executable(auth ${SOURCE_FILES})

target_link_libraries(auth wthttp wt boost_system wtdbo wtdbosqlite3)