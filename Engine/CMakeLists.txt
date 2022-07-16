add_executable(${ENGINE_NAME} "")

target_link_libraries(${ENGINE_NAME} PRIVATE
	glad
    	glfw
	glm
	imgui
)

add_subdirectory(Source)
