add_executable(${APP_NAME} "")

target_link_libraries(${APP_NAME} PRIVATE
	glad
    	glfw
	glm
	imgui
)

add_subdirectory(Source)
