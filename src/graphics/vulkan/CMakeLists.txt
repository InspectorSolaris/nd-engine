set(TARGET_NAME nd-src-graphics-vulkan)
set(TARGET_SRC
    buffer.cpp
    buffer_view.cpp
    command_buffer.cpp
    command_pool.cpp
    context.cpp
    descriptor_pool.cpp
    descriptor_set.cpp
    descriptor_set_layout.cpp
    device.cpp
    framebuffer.cpp
    image.cpp
    image_view.cpp
    instance.cpp
    pipeline.cpp
    pipeline_layout.cpp
    render_pass.cpp
    shader_module.cpp
    shared.cpp
    surface.cpp
    swapchain.cpp)

add_library(${TARGET_NAME} STATIC ${TARGET_SRC})

target_link_libraries(${TARGET_NAME}
    PRIVATE nd-src-libs-vulkan
    PRIVATE nd-src-tools)

target_precompile_headers(${TARGET_NAME} PRIVATE pch.hpp)
