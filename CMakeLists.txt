cmake_minimum_required(VERSION 3.8)
project(team18)

INCLUDE_DIRECTORIES(/usr/local/include)
link_directories(/usr/local/lib)

set(CMAKE_CXX_STANDARD 11)

FILE (COPY "templates.xml" DESTINATION ".")

set(SOURCE_FILES
        src/Auth1.cpp
        src/AuthWidget.cpp
        src/AuthWidget.h
        src/Bridge.cpp
        src/Bridge.h
        src/RegistrationView.cpp
        src/RegistrationView.h
        src/Session.cpp
        src/Session.h
        src/User.cpp
        src/User.h
        src/UserDetailsModel.cpp
        src/UserDetailsModel.h
        src/css/styleSheet.css
        )

add_executable(team18 ${SOURCE_FILES})

target_link_libraries(team18 wthttp wt boost_system wtdbo wtdbosqlite3)