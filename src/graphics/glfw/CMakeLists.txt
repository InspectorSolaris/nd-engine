set(TARGET_NAME nd-src-graphics-glfw)
set(TARGET_SRC
    glfw_vulkan.cpp
    window_init.cpp)

add_library(${TARGET_NAME} ${TARGET_SRC})

target_link_libraries(${TARGET_NAME}
    PUBLIC nd-src-libs-glfw
    PUBLIC nd-src-libs-vulkan
    PUBLIC nd-src-graphics-vulkan
    PUBLIC nd-src-tools)

target_include_directories(${TARGET_NAME} INTERFACE "")
target_precompile_headers(${TARGET_NAME} PRIVATE pch.hpp)
