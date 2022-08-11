add_executable(${SANDBOX_TARGET_NAME} "")

target_link_libraries(${SANDBOX_TARGET_NAME} PRIVATE
	Engine
	spdlog
)

add_subdirectory(Source)

GroupSourcesByFolder(${SANDBOX_TARGET_NAME})

set (Resources shader.glsl)

get_property(GENERATOR_IS_MULTI_CONFIG GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)
set(TO_COPY "${CMAKE_CURRENT_SOURCE_DIR}/$<JOIN:${Resources},;${CMAKE_CURRENT_SOURCE_DIR}/>")
set(COPY_TO "${CMAKE_CURRENT_BINARY_DIR}/$<$<BOOL:${GENERATOR_IS_MULTI_CONFIG}>:$<CONFIG>>")
set(OUTPUT_AND_DEPENDS "${COPY_TO}/$<JOIN:${Resources},;${COPY_TO}/>")
add_custom_command(
  OUTPUT "${OUTPUT_AND_DEPENDS}"
  COMMAND ${CMAKE_COMMAND}
    ARGS
      -E copy_if_different
      "${TO_COPY}"
      "${COPY_TO}"
  COMMENT "Copying resources for ${SANDBOX_TARGET_NAME}"
  COMMAND_EXPAND_LISTS
  DEPENDS ${Resources}
)
add_custom_target(${SANDBOX_TARGET_NAME}-CopyResources
  DEPENDS "${OUTPUT_AND_DEPENDS}"
)
add_dependencies(${SANDBOX_TARGET_NAME}
  ${SANDBOX_TARGET_NAME}-CopyResources
)