cmake_minimum_required(VERSION 3.10)
project(HelloLibraryProject)

# Create the library
add_library(MyHelloLibrary HelloLib/Hello.cpp HelloLib/Hello.h)

# Tell CMake where to find the .h file
target_include_directories(MyHelloLibrary PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/HelloLib)

# Create the executable and link the library
add_executable(hello_app src/main.cpp)
target_link_libraries(hello_app MyHelloLibrary)