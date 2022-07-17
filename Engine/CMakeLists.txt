add_library(${ENGINE_TARGET_NAME} STATIC "")

target_link_libraries(${ENGINE_TARGET_NAME} PRIVATE
	glad
    	glfw
	glm
	imgui
	spdlog
)

add_subdirectory(Source)

GroupSourcesByFolder(${ENGINE_TARGET_NAME})
