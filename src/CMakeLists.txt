set(TARGET_NAME nd-src)
set(TARGET_SRC main.cpp)

add_subdirectory(libs)
add_subdirectory(tools)
add_subdirectory(core)
add_subdirectory(graphics)

add_executable(${TARGET_NAME} ${TARGET_SRC})

target_link_libraries(${TARGET_NAME}
    PRIVATE ${TARGET_NAME}-tools
    PRIVATE ${TARGET_NAME}-core
    PRIVATE ${TARGET_NAME}-graphics)
