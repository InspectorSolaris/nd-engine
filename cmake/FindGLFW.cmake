if(WIN32)
    
elseif(UNIX)
    find_path(GLFW_INCLUDE_DIR
        NAMES
            GLFW/glfw3.h
            GLFW/glfw3native.h
        PATHS
            /usr/include
            /usr/local/include
        DOC
            "GLFW include directory")
    find_library(GLFW_LIBRARY
        NAMES
            glfw glfw3
        PATHS
            /lib
            /lib64
            /usr/lib
            /usr/lib64
            /usr/local/lib
            /usr/local/lib64
        DOC
            "GLFW library")
endif()

set(GLFW_LIBRARIES ${GLFW_LIBRARY})
set(GLFW_INCLUDE_DIRS ${GLFW_INCLUDE_DIR})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(GLFW
    DEFAULT_MSG
    GLFW_LIBRARY GLFW_INCLUDE_DIR)

mark_as_advanced(GLFW_INCLUDE_DIR GLFW_LIBRARY)

if(GLFW_FOUND AND NOT TARGET GLFW::GLFW)
    add_library(GLFW::GLFW UNKNOWN IMPORTED)
    set_target_properties(GLFW::GLFW PROPERTIES
        IMPORTED_LOCATION ${GLFW_LIBRARIES}
        INTERFACE_INCLUDE_DIRECTORIES ${GLFW_INCLUDE_DIRS})
endif()

if(GLFW_FOUND AND NOT TARGET GLFW::Headers)
    add_library(GLFW::Headers INTERFACE IMPORTED)
    set_target_properties(GLFW::Headers PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES ${GLFW_INCLUDE_DIRS})
endif()
