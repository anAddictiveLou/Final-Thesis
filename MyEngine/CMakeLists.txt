# root CMakeLists.txt
cmake_minimum_required(VERSION 3.4.3 FATAL_ERROR)
set(HIDAPI_WITH_LIBUSB FALSE) # surely will be used only on Linux
set(BUILD_SHARED_LIBS FALSE) # HIDAPI as static library on all platforms

add_subdirectory(hidapi)

# Include directories for your application
include_directories(src inc)  # Add the 'inc' directory

# my_application/CMakeLists.txt
project(MyEngine)

add_executable(MyEngine main.cpp src/core.cpp src/byte.cpp)

# NOTE: no `find_package` is required, since HIDAPI targets are already a part of the project tree
target_link_libraries(MyEngine PRIVATE hidapi::hidapi)