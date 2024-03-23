cmake_minimum_required(VERSION 3.23.2)

project(ICVP_TEST)

set(CMAKE_CXX_STANDARD 17)

file(GLOB LOCAL_INCLUDE_DIR public/** test/** include/**)
file(GLOB LOCAL_SOURCE_DIR public/**/*.cpp test/**/*.cpp)

list(APPEND THIRD_INCLUDE_DIR ./include/third_party)


message(STATUS "LOCAL_INCLUDE_DIR = ${LOCAL_INCLUDE_DIR}")


include(FetchContent)

Set(FETCHCONTENT_QUIET FALSE)
# set FETCHCONTENT_UPDATES_DISCONNECTED to OFF to get latest repo
Set(FETCHCONTENT_UPDATES_DISCONNECTED ON)

function(load_module name uri success)
  FetchContent_GetProperties(${name})

  if(NOT ${${name}_POPULATED})
    FetchContent_Declare(
      ${name}
      GIT_REPOSITORY ${uri}
      GIT_PROGRESS TRUE
    )
    message(STATUS "Cannot find module ${name}, start downloading...")
    FetchContent_MakeAvailable(${name})
  endif()

  FetchContent_GetProperties(glfw)
  if (${${name}_POPULATED})
    Set(${success} TRUE PARENT_SCOPE)
  else()
    Set(${success} FALSE PARENT_SCOPE)
  endif()
endfunction()

set(load_ret FALSE)
load_module(glfw git@github.com:glfw/glfw.git load_ret)
if (NOT ${load_ret})
  message(FATAL_ERROR "Download glfw failed.")
  return()
else()
  list(APPEND THIRD_INCLUDE_DIR ${glfw_SOURCE_DIR}/include)
endif()

load_module(glm git@github.com:g-truc/glm.git load_ret)
if (NOT ${load_ret})
  message(FATAL_ERROR "Download glm failed.")
  return()
else()
list(APPEND THIRD_INCLUDE_DIR ${glm_SOURCE_DIR})
endif()

add_executable(ICVP_TEST 
  main.cpp
  glad.c
  ${LOCAL_SOURCE_DIR}
)

target_include_directories(ICVP_TEST PUBLIC 
  ${LOCAL_INCLUDE_DIR}
)
target_include_directories(ICVP_TEST SYSTEM PUBLIC 
  ${THIRD_INCLUDE_DIR}
)

target_link_libraries(ICVP_TEST 
  glfw
)