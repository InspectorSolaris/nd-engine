set(TARGET_NAME nd-src-graphics-vulkan)
set(TARGET_SRC
    buffer.cpp
    buffer_view.cpp
    command_buffer.cpp
    command_pool.cpp
    context_configuration.cpp
    context.cpp
    descriptor_pool.cpp
    descriptor_set.cpp
    descriptor_set_layout.cpp
    device.cpp
    framebuffer.cpp
    image.cpp
    image_view.cpp
    instance.cpp
    pipeline_layout.cpp
    pipeline.cpp
    queue.cpp
    render_pass.cpp
    shader_module.cpp
    shared.cpp
    surface.cpp
    swapchain.cpp
    synchronization.cpp)

set(SHADERS_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/shaders)
set(SHADERS_BIN_DIR ${CMAKE_CURRENT_BINARY_DIR}/shaders)

set(SHADERS_NAME nd-src-graphics-vulkan-shaders)
set(SHADERS_SRC
    ${SHADERS_SRC_DIR}/vert.vert
    ${SHADERS_SRC_DIR}/frag.frag)

add_library(${TARGET_NAME} STATIC ${TARGET_SRC})

target_link_libraries(${TARGET_NAME}
    PRIVATE nd-src-libs-vulkan
    PRIVATE nd-src-tools)

target_precompile_headers(${TARGET_NAME} PRIVATE pch.hpp)

add_custom_command(
    OUTPUT ${SHADERS_BIN_DIR}
    COMMAND ${CMAKE_COMMAND} -E make_directory ${SHADERS_BIN_DIR}
    COMMENT "Creating ${SHADERS_BIN_DIR} directory")

foreach(SHADER_SRC IN LISTS SHADERS_SRC)
    get_filename_component(SHADER_SRC_NAME ${SHADER_SRC} NAME_WE)

    set(SHADER_BIN ${SHADERS_BIN_DIR}/${SHADER_SRC_NAME}.spv)

    add_custom_command(
        OUTPUT ${SHADER_BIN}
        COMMAND ${Vulkan_GLSLC_EXECUTABLE} ${SHADER_SRC} -o ${SHADER_BIN}
        COMMENT "Compiling ${SHADER_SRC} shader"
        DEPENDS ${SHADERS_BIN_DIR} ${SHADER_SRC})

    list(APPEND SHADERS_BIN ${SHADER_BIN})
endforeach(SHADER_SRC)

add_custom_target(${SHADERS_NAME} DEPENDS ${SHADERS_BIN})

add_dependencies(${TARGET_NAME} ${SHADERS_NAME})
