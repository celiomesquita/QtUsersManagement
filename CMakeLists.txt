cmake_minimum_required(VERSION 3.5)

project(Sirerc VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Sql)

# Find mongocxx and bsoncxx libraries
find_package(mongocxx REQUIRED)
find_package(bsoncxx REQUIRED)

target_link_libraries(Sirerc PRIVATE mongocxx bsoncxx)

add_definitions(-DDB_PATH=\"${CMAKE_SOURCE_DIR}/users.db\")

set(PROJECT_SOURCES
    main.cpp
    MainWindow.cpp
    Database.cpp
    HashUtils.cpp
    UserDB.cpp
    LoginDialog.cpp
    UserManagementDialog.cpp

    MainWindow.h
    Database.h
    HashUtils.h
    UserDB.h
    LoginDialog.h
    UserManagementDialog.h

    LoginDialog.ui
    UserManagementDialog.ui
    MainWindow.ui
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(Sirerc
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
else()
    if(ANDROID)
        add_library(Sirerc SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(Sirerc
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(Sirerc PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Sql)

set_target_properties(Sirerc PROPERTIES
    WIN32_EXECUTABLE TRUE
)

install(TARGETS Sirerc
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(Sirerc)
endif()
