cmake_minimum_required(VERSION 3.12)
project(llpydoocs)

# Set the PKG_CONFIG_PATH environment variable
set(ENV{PKG_CONFIG_PATH} "/local/Darwin-x86_64/lib/pkgconfig/:$ENV{PKG_CONFIG_PATH}")

# Find pybind11
find_package(pybind11 REQUIRED)

# Use pkg-config to get the flags for doocsapi
find_package(PkgConfig REQUIRED)
pkg_check_modules(DOOCSAPI REQUIRED IMPORTED_TARGET doocs-doocsapi)

# Define the source file
add_executable(llpydoocs src/llpydoocs.cpp)

# Link libraries
target_link_libraries(llpydoocs PRIVATE pybind11::pybind11 PkgConfig::DOOCSAPI)
