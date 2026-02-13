cmake_minimum_required(VERSION 3.10)
project(Worksheet5_Installer)

# Set the installer type based on the OS
if (WIN32)
    set(CPACK_GENERATOR "NSIS")
else ()
    set(CPACK_GENERATOR "ZIP")
endif ()

include(CPack)

# Create the library and executable
add_library(maths adder.cpp)
add_executable(calc calc.cpp)
target_link_libraries(calc maths)

# Define where the files go when installed
install(TARGETS calc maths
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib/static
)