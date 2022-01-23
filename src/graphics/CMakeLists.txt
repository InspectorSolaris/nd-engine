set(TARGET_NAME nd-src-graphics)
set(TARGET_SRC)

add_subdirectory(vulkan)
add_subdirectory(glfw)

add_library(${TARGET_NAME} INTERFACE ${TARGET_SRC})

target_include_directories(${TARGET_NAME} INTERFACE "")

target_link_libraries(${TARGET_NAME}
    INTERFACE ${TARGET_NAME}-vulkan
    INTERFACE ${TARGET_NAME}-glfw)
