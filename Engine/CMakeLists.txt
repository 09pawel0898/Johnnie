add_library(${ENGINE_TARGET_NAME} STATIC "")

target_link_libraries(${ENGINE_TARGET_NAME} PRIVATE
	glad
    	glfw
	spdlog
	stb_image
	stduuid
	assimp
)


target_link_libraries(${ENGINE_TARGET_NAME} PUBLIC
	imgui
	glm
)

add_subdirectory(Source)
add_subdirectory(Include)

target_include_directories(${ENGINE_TARGET_NAME} PUBLIC Include)
target_include_directories(${ENGINE_TARGET_NAME} PUBLIC Source)

GroupSourcesByFolder(${ENGINE_TARGET_NAME})

if (MSVC)
   set_target_properties(${ENGINE_TARGET_NAME} PROPERTIES COMPILE_FLAGS "/YuEnginePCH.hpp")
   set_source_files_properties(Source/EnginePCH.cpp PROPERTIES COMPILE_FLAGS "/YcEnginePCH.hpp")
endif(MSVC)