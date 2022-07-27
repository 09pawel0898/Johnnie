add_library(${ENGINE_TARGET_NAME} STATIC "")

target_link_libraries(${ENGINE_TARGET_NAME} PRIVATE
	glad
    	glfw
	glm
	spdlog
)


target_link_libraries(${ENGINE_TARGET_NAME} PUBLIC
	imgui
)

add_subdirectory(Source)
add_subdirectory(Include)

target_include_directories(${ENGINE_TARGET_NAME} PUBLIC Include)
target_include_directories(${ENGINE_TARGET_NAME} PUBLIC Source)

GroupSourcesByFolder(${ENGINE_TARGET_NAME})
