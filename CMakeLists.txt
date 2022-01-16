cmake_minimum_required(VERSION 3.21)

# set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CMAKE_COMMAND} -E time")
# set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK "${CMAKE_COMMAND} -E time")

set(CHECKS -*,clang-analyzer-optin.performance.*,performance-*)

set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type")
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE CACHE BOOL "Export compile commands")

set(CMAKE_C_CLANG_TIDY clang-tidy -checks=${CHECKS})
set(CMAKE_C_COMPILER clang CACHE STRING "C compiler")
set(CMAKE_C_STANDARD 17 CACHE STRING "C standard")
set(CMAKE_C_STANDARD_REQUIRED TRUE CACHE BOOL "C standard required")

set(CMAKE_CXX_CLANG_TIDY clang-tidy -checks=${CHECKS})
set(CMAKE_CXX_COMPILER clang++ CACHE STRING "C++ compier")
set(CMAKE_CXX_STANDARD 17 CACHE STRING "C++ standard")
set(CMAKE_CXX_STANDARD_REQUIRED TRUE CACHE BOOL "C++ standard required")

project(nd-engine)

add_subdirectory(src)
