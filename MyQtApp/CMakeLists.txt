cmake_minimum_required(VERSION 3.19)
project(MyQtApp LANGUAGES CXX)

# Find the necessary Qt6 components
find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets)

# 1. Define the complete list of project files FIRST
set(PROJECT_SOURCES
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    ModelPart.cpp        # Added
    ModelPart.h          # Added
    ModelPartList.cpp    # Added
    ModelPartList.h      # Added
)

# Standard Qt project setup
qt_standard_project_setup()

# 2. Tell the executable to use the PROJECT_SOURCES variable
qt_add_executable(MyQtApp
    WIN32 MACOSX_BUNDLE
    ${PROJECT_SOURCES}
)

# Link the libraries
target_link_libraries(MyQtApp
    PRIVATE
        Qt::Core
        Qt::Widgets
)

include(GNUInstallDirs)

install(TARGETS MyQtApp
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET MyQtApp
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})