set(TARGET_NAME nd-src-core)
set(TARGET_SRC)

add_library(${TARGET_NAME} INTERFACE ${TARGET_SRC})

target_include_directories(${TARGET_NAME} INTERFACE "")
