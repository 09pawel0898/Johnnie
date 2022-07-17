cmake_minimum_required(VERSION 3.2)

# Project Organization

function(GroupSourcesByFolder target)
  set(SOURCE_GROUP_DELIMITER "/")
  set(last_dir "")
  set(files "")

  get_target_property(sources ${target} SOURCES)

  foreach(file ${sources})
    file(RELATIVE_PATH relative_file "${PROJECT_SOURCE_DIR}" ${file})
    get_filename_component(dir "${relative_file}" PATH)
    if(NOT "${dir}" STREQUAL "${last_dir}")
      if(files)
        source_group("${last_dir}" FILES ${files})
      endif()
      set(files "")
    endif()
    set(files ${files} ${file})
    set(last_dir "${dir}")
  endforeach()

  if(files)
    source_group("${last_dir}" FILES ${files})
  endif()
endfunction()

# Project

set(PROJECT_NAME Project)
project(${PROJECT_NAME} C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Targets

set(SANDBOX_TARGET_NAME Johnnie)
set(ENGINE_TARGET_NAME Engine)

# Dependencies

set(DEPENDENCIES_DIR_NAME Dependencies)
add_subdirectory(Dependencies)

# Targets Subdirectories

add_subdirectory(${ENGINE_TARGET_NAME})
add_subdirectory(${SANDBOX_TARGET_NAME})

if (MSVC)
    set_property(DIRECTORY ${PROJECT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${SANDBOX_TARGET_NAME})
endif()