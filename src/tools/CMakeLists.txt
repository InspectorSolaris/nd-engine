set(TARGET_NAME nd-src-tools)
set(TARGET_SRC
    scope.cpp
    tools.cpp)

add_library(${TARGET_NAME} ${TARGET_SRC})

target_include_directories(${TARGET_NAME} INTERFACE "")

target_link_libraries(${TARGET_NAME}
    PUBLIC nd-src-libs-fmt
    PUBLIC nd-src-libs-spdlog
    PUBLIC nd-src-libs-gsl)

target_precompile_headers(${TARGET_NAME} PRIVATE pch.hpp)
