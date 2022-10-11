add_executable(${SANDBOX_TARGET_NAME} "")

target_link_libraries(${SANDBOX_TARGET_NAME} PRIVATE
	Engine
	spdlog
)

add_subdirectory(Source)

GroupSourcesByFolder(${SANDBOX_TARGET_NAME})

add_custom_command(
	TARGET ${SANDBOX_TARGET_NAME} POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory
	${CMAKE_CURRENT_SOURCE_DIR}/Assets $<TARGET_FILE_DIR:${SANDBOX_TARGET_NAME}>/Assets
	COMMENT "Copying Johnnie resources for ${SANDBOX_TARGET_NAME} into binary directory."
	COMMAND_EXPAND_LISTS
)

add_custom_command(
	TARGET ${SANDBOX_TARGET_NAME} POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory
	${CMAKE_CURRENT_SOURCE_DIR}/Assets $<TARGET_FILE_DIR:${SANDBOX_TARGET_NAME}>/../Assets
	COMMENT "Copying Johnnie resources for ${SANDBOX_TARGET_NAME} into build directory."
	COMMAND_EXPAND_LISTS
)


add_custom_command(
	TARGET ${SANDBOX_TARGET_NAME} POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory
	${CMAKE_CURRENT_SOURCE_DIR}/../Engine/Assets $<TARGET_FILE_DIR:${SANDBOX_TARGET_NAME}>/Assets
	COMMENT "Copying Engine resources for ${SANDBOX_TARGET_NAME} into binary directory."
	COMMAND_EXPAND_LISTS
)

add_custom_command(
	TARGET ${SANDBOX_TARGET_NAME} POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory
	${CMAKE_CURRENT_SOURCE_DIR}/../Engine/Assets $<TARGET_FILE_DIR:${SANDBOX_TARGET_NAME}>/../Assets
	COMMENT "Copying Engine resources for ${SANDBOX_TARGET_NAME} into build directory."
	COMMAND_EXPAND_LISTS
)