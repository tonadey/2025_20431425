cmake_minimum_required(VERSION 3.10)
project(SubdirectoryProject)

# Tell CMake to go into the HelloLib folder and run the CMakeLists.txt there
add_subdirectory(HelloLib)

# Now build the main app
add_executable(hello_app src/main.cpp)

# Link to the library (CMake remembers it from the subdirectory!)
target_link_libraries(hello_app MyHelloLibrary)