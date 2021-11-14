set(TARGET_NAME nd-src-graphics-glfw)
set(TARGET_SRC
    context.cpp
    window.cpp)

add_library(${TARGET_NAME} STATIC ${TARGET_SRC})

target_link_libraries(${TARGET_NAME}
    PRIVATE nd-src-libs-glfw
    PRIVATE nd-src-libs-vulkan
    PRIVATE nd-src-tools)

target_precompile_headers(${TARGET_NAME} PRIVATE pch.hpp)
