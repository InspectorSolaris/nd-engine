if(WIN32)
    
elseif(UNIX)
    find_path(GLM_INCLUDE_DIR
        NAMES
            glm/glm.hpp
        PATHS
            /usr/include
            /usr/local/include
        DOC
            "GLM include directory")
endif()

set(GLM_INCLUDE_DIRS ${GLM_INCLUDE_DIR})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(GLM
    DEFAULT_MSG
    GLM_INCLUDE_DIR)

mark_as_advanced(GLM_INCLUDE_DIR)

if(GLM_FOUND AND NOT TARGET GLM::Headers)
    add_library(GLM::Headers INTERFACE IMPORTED)
    set_target_properties(GLM::Headers PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES ${GLM_INCLUDE_DIRS})
endif()
