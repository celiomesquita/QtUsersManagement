cmake_minimum_required(VERSION 3.10)

project(Sirerc VERSION 0.1 LANGUAGES CXX)

# Enable automatic UIC, MOC, and RCC processing
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# Set C++ standard to C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set the prefix path for Qt5 installation
set(CMAKE_PREFIX_PATH "C:/Qt/5.15.2/msvc2019_64/lib/cmake")

# Find required Qt5 components
find_package(Qt5 REQUIRED COMPONENTS Widgets Sql)

# Add a definition for the database path
add_definitions(-DDB_PATH=\"${CMAKE_SOURCE_DIR}/Database/Database.db\")

# List the source files for the project
set(PROJECT_SOURCES

    Database/Database.cpp
    Database/Database.h

    HashUtils.cpp
    HashUtils.h

    Login/LoginDialog.cpp
    Login/LoginDialog.h
    Login/LoginDialog.ui

    main.cpp
    MainWindow.cpp
    MainWindow.h
    MainWindow.ui

    User/UserDB.cpp
    User/UserDB.h
    User/UserManager.cpp
    User/UserManager.h
    User/UserManager.ui

    Project/ProjectDB.cpp
    Project/ProjectDB.h
    Project/ProjectManager.cpp
    Project/ProjectManager.h
    Project/ProjectManager.ui
)

# Add the executable
add_executable(Sirerc ${PROJECT_SOURCES})

# Link the necessary Qt5 libraries
target_link_libraries(Sirerc PRIVATE Qt5::Widgets Qt5::Sql)

# Set executable properties for Windows
set_target_properties(Sirerc PROPERTIES
    WIN32_EXECUTABLE TRUE
)

# Install targets
install(TARGETS Sirerc
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
