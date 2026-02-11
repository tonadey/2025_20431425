# This file only cares about the library
add_library(MyHelloLibrary Hello.cpp Hello.h)
target_include_directories(MyHelloLibrary PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})