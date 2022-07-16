add_executable(${APP_NAME} "")

target_link_libraries(${APP_NAME} PRIVATE
	Engine
	spdlog
)

add_subdirectory(Source)

GroupSourcesByFolder(${APP_NAME})