add_executable(${SANDBOX_TARGET_NAME} "")

target_link_libraries(${SANDBOX_TARGET_NAME} PRIVATE
	Engine
	spdlog
)

add_subdirectory(Source)

GroupSourcesByFolder(${SANDBOX_TARGET_NAME})