set(TARGET_NAME nd-src-graphics-glfw)
set(TARGET_SRC
    glfw_vulkan.cpp
    window.cpp)

add_library(${TARGET_NAME} ${TARGET_SRC})

target_link_libraries(${TARGET_NAME}
    PUBLIC nd-src-libs-glfw
    PUBLIC nd-src-libs-vulkan
    PUBLIC nd-src-graphics-vulkan
    PUBLIC nd-src-tools)

target_precompile_headers(${TARGET_NAME} PRIVATE pch.hpp)
