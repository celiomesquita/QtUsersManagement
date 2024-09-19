cmake_minimum_required(VERSION 3.10)

project(Sirerc VERSION 0.1 LANGUAGES CXX)

# Enable automatic UIC, MOC, and RCC processing
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# Set C++ standard to C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# set(CMAKE_PREFIX_PATH "C:/Qt/5.15.2/msvc2019_64/lib/cmake")
# set(CMAKE_PREFIX_PATH "/usr/lib/qt5/bin/qmake")

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

    resources.qrc
)

# Translation files
set(TS_FILES
    ${CMAKE_SOURCE_DIR}/translations/translation_en.ts
    ${CMAKE_SOURCE_DIR}/translations/translation_pt.ts
)

file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/translations)

# set(QT_LRELEASE_EXECUTABLE "C:/Qt/5.15.2/msvc2019_64/bin")
# set(QT_LRELEASE_EXECUTABLE "/usr/lib/qt5/bin")


# Find the lrelease tool
find_program(LRELEASE_EXECUTABLE lrelease)

if(NOT LRELEASE_EXECUTABLE)
    message(FATAL_ERROR "lrelease executable not found, make sure Qt tools are installed.")
endif()

# Generate .qm files from .ts files
foreach(TS_FILE ${TS_FILES})
    get_filename_component(QM_FILE ${TS_FILE} NAME_WE)
    set(QM_FILE translations/${QM_FILE}.qm)

    add_custom_command(
        OUTPUT ${QM_FILE}
        COMMAND ${LRELEASE_EXECUTABLE} ${TS_FILE} -qm ${QM_FILE}
        DEPENDS ${TS_FILE}
        COMMENT "Generating ${QM_FILE} from ${TS_FILE}"
    )

    list(APPEND QM_FILES ${QM_FILE})
endforeach()

# Ensure that the generated QM files are part of the build
add_custom_target(translations ALL DEPENDS ${QM_FILES})


# Add the executable
add_executable(Sirerc ${PROJECT_SOURCES})

# Set executable properties for Windows (Optional, only relevant for Windows)
set_target_properties(Sirerc PROPERTIES
    WIN32_EXECUTABLE TRUE
)

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
