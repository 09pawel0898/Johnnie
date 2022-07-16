add_library(${ENGINE_NAME} STATIC "")

target_link_libraries(${ENGINE_NAME} PRIVATE
	glad
    	glfw
	glm
	imgui
	spdlog
)

add_subdirectory(Source)

GroupSourcesByFolder(${ENGINE_NAME})
