add_library(${ENGINE_TARGET_NAME} STATIC "")

target_link_libraries(${ENGINE_TARGET_NAME} PRIVATE
	glad
    	glfw
	glm
	imgui
	spdlog
)

add_subdirectory(Source)
add_subdirectory(Include)

target_include_directories(${ENGINE_TARGET_NAME} PUBLIC Include)

GroupSourcesByFolder(${ENGINE_TARGET_NAME})
