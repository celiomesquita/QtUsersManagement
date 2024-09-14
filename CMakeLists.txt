cmake_minimum_required(VERSION 3.10) # Recommended to upgrade CMake version

project(Sirerc VERSION 0.1 LANGUAGES CXX)

# Enable automatic UIC, MOC, and RCC processing
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# Set C++ standard to C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find Qt5 components
find_package(QT NAMES Qt5 REQUIRED COMPONENTS Widgets Sql)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Sql)

# Add a definition for the database path
add_definitions(-DDB_PATH=\"${CMAKE_SOURCE_DIR}/Database/users.db\")

# Source files for the project
set(PROJECT_SOURCES
    Managers/main.cpp
    Managers/MainWindow.cpp
    Managers/MainWindow.h
    Managers/MainWindow.ui

    Database/Database.cpp
    Database/Database.h
    Database/UserDB.cpp
    Database/UserDB.h

    Hash/HashUtils.cpp
    Hash/HashUtils.h

    Login/LoginDialog.cpp
    Login/LoginDialog.h
    Login/LoginDialog.ui

    Management/UserManagementDialog.cpp
    Management/UserManagementDialog.h
    Management/UserManagementDialog.ui

    Project
)

# Handle different Qt versions
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

# Link Qt libraries and MongoDB libraries
target_link_libraries(Sirerc PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Sql)

# Set executable properties (Windows)
set_target_properties(Sirerc PROPERTIES
    WIN32_EXECUTABLE TRUE
)

# Install targets
install(TARGETS Sirerc
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# Finalize executable for Qt6
if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(Sirerc)
endif()
