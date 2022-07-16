if (NOT TARGET uninstall)
    add_custom_target(uninstall)
    set_target_properties(
    	uninstall
        PROPERTIES FOLDER ${DEPENDENCIES_DIR_NAME}
)
endif()

# GLAD

add_subdirectory(glad)
set_target_properties(
	glad
	PROPERTIES FOLDER ${DEPENDENCIES_DIR_NAME}/glad
)

# GLM

add_subdirectory(glm)
set_target_properties(
	glm
    PROPERTIES FOLDER ${DEPENDENCIES_DIR_NAME}/glm
)

# IMGUI

add_subdirectory(imgui)
set_target_properties(
    imgui
    PROPERTIES FOLDER ${DEPENDENCIES_DIR_NAME}/imgui
)

# SPDLOG

option(SPDLOG_FMT_EXTERNAL_HO "Use external fmt header-only library instead of bundled" ON) 
if(SPDLOG_FMT_EXTERNAL AND SPDLOG_FMT_EXTERNAL_HO) 
	message(FATAL_ERROR "SPDLOG_FMT_EXTERNAL and SPDLOG_FMT_EXTERNAL_HO are mutually exclusive") 
endif() 

add_subdirectory(spdlog)
set_target_properties(
    spdlog
    PROPERTIES FOLDER ${DEPENDENCIES_DIR_NAME}/spdlog
)


# GLFW

add_subdirectory(glfw)
set(GLFW_BUILD_EXAMPLES  OFF CACHE INTERNAL "")
set(GLFW_BUILD_TESTS     OFF CACHE INTERNAL "")
set(GLFW_BUILD_DOCS      OFF CACHE INTERNAL "")
set(GLFW_INSTALL         OFF CACHE INTERNAL "")

set_target_properties(
    glfw
    update_mappings
    PROPERTIES FOLDER ${DEPENDENCIES_DIR_NAME}/glfw
)