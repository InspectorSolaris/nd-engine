set(LIBS_NAME nd-src-libs)

list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "Glfw Docs" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "Glfw Tests" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "Glfw Examples" FORCE)

add_subdirectory(glfw)

find_package(Vulkan REQUIRED)
find_package(GLM REQUIRED)
find_package(fmt REQUIRED)
find_package(spdlog REQUIRED)

add_library(${LIBS_NAME}-vulkan INTERFACE)
add_library(${LIBS_NAME}-vulkan-headers INTERFACE)
add_library(${LIBS_NAME}-glfw INTERFACE)
add_library(${LIBS_NAME}-glm INTERFACE)
add_library(${LIBS_NAME}-fmt INTERFACE)
add_library(${LIBS_NAME}-fmt-headers INTERFACE)
add_library(${LIBS_NAME}-spdlog INTERFACE)
add_library(${LIBS_NAME}-spdlog-headers INTERFACE)

target_link_libraries(${LIBS_NAME}-vulkan INTERFACE Vulkan::Vulkan)
target_link_libraries(${LIBS_NAME}-vulkan-headers INTERFACE Vulkan::Headers)
target_link_libraries(${LIBS_NAME}-glfw INTERFACE glfw)
target_link_libraries(${LIBS_NAME}-glm INTERFACE GLM::Headers)
target_link_libraries(${LIBS_NAME}-fmt INTERFACE fmt::fmt)
target_link_libraries(${LIBS_NAME}-fmt-headers INTERFACE fmt::fmt-header-only)
target_link_libraries(${LIBS_NAME}-spdlog INTERFACE spdlog::spdlog)
target_link_libraries(${LIBS_NAME}-spdlog-headers INTERFACE spdlog::spdlog_header_only)
