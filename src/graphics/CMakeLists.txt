set(TARGET_NAME nd-src-graphics)
set(TARGET_SRC
    render_context.cpp
    render.cpp
    scene.cpp)

add_subdirectory(vulkan)
add_subdirectory(glfw)

add_library(${TARGET_NAME} ${TARGET_SRC})

target_link_libraries(${TARGET_NAME}
    PUBLIC ${TARGET_NAME}-vulkan
    PUBLIC ${TARGET_NAME}-glfw)

target_include_directories(${TARGET_NAME} INTERFACE "")
target_precompile_headers(${TARGET_NAME} PRIVATE pch.hpp)

