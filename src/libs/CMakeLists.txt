set(LIBS_NAME nd-src-libs)

list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "Glfw Docs" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "Glfw Tests" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "Glfw Examples" FORCE)
set(SPDLOG_FMT_EXTERNAL ON CACHE BOOL "Spdlog Fmt External" FORCE)

find_package(Vulkan REQUIRED)
find_package(GLFW)
find_package(GLM)
find_package(fmt)
find_package(spdlog)
find_package(GSL)

if(NOT TARGET glfw)
    add_subdirectory(glfw)
endif()

if(NOT TARGET glm)
    add_subdirectory(glm)
endif()

if(NOT TARGET fmt::fmt)
    add_subdirectory(fmt)
endif()

if(NOT TARGET spdlog::spdlog)
    add_subdirectory(spdlog)
endif()

if(NOT TARGET gsl)
    add_subdirectory(gsl)
endif()

add_library(${LIBS_NAME}-vulkan INTERFACE)
add_library(${LIBS_NAME}-glfw INTERFACE)
add_library(${LIBS_NAME}-glm INTERFACE)
add_library(${LIBS_NAME}-fmt INTERFACE)
add_library(${LIBS_NAME}-spdlog INTERFACE)
add_library(${LIBS_NAME}-gsl INTERFACE)

target_link_libraries(${LIBS_NAME}-vulkan INTERFACE Vulkan::Vulkan)
target_link_libraries(${LIBS_NAME}-glfw INTERFACE glfw)
target_link_libraries(${LIBS_NAME}-glm INTERFACE glm::glm)
target_link_libraries(${LIBS_NAME}-fmt INTERFACE fmt::fmt)
target_link_libraries(${LIBS_NAME}-spdlog INTERFACE spdlog::spdlog)
target_link_libraries(${LIBS_NAME}-gsl INTERFACE Microsoft.GSL::GSL)
