set(TARGET_NAME nd-src-graphics-vulkan)
set(TARGET_SRC
    buffer_init.cpp
    command_init.cpp
    descriptor_init.cpp
    device_init.cpp
    image_init.cpp
    instance_init.cpp
    memory_init.cpp
    objects_cfg_builder.cpp
    objects_cfg.cpp
    objects_init_builder.cpp
    objects_init.cpp
    objects_complete.cpp
    objects.cpp
    pipeline_init.cpp
    queue_init.cpp
    render_pass_init.cpp
    render.cpp
    shader_module_init.cpp
    shared_init.cpp
    surface_init.cpp
    swapchain_init.cpp
    sync_init.cpp)

set(SHADERS_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/shaders)
set(SHADERS_BIN_DIR ${CMAKE_CURRENT_BINARY_DIR}/shaders)

set(SHADERS_NAME nd-src-graphics-vulkan-shaders)
set(SHADERS_SRC
    ${SHADERS_SRC_DIR}/vert.vert
    ${SHADERS_SRC_DIR}/frag.frag)

add_library(${TARGET_NAME} ${TARGET_SRC})

target_link_libraries(${TARGET_NAME}
    PUBLIC nd-src-libs-vulkan
    PUBLIC nd-src-libs-glm
    PUBLIC nd-src-tools)

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
