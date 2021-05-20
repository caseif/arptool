cmake_minimum_required(VERSION 3.13)
if(POLICY CMP0048)
  cmake_policy(SET CMP0048 NEW)
endif()

project(arptool VERSION 1.0.0)

set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)
if("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")
  message(SEND_ERROR "In-source builds are not allowed.")
endif()

if(NOT CMAKE_SIZEOF_VOID_P EQUAL 8)
  message(SEND_ERROR "32-bit builds are not supported")
endif()

set(CMAKE_CXX_OUTPUT_EXTENSION_REPLACE 1)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(EXT_LIBS "")

set(LIBARP_ROOT_DIR "${PROJECT_SOURCE_DIR}/libs/libarp")
set(LIBARP_SOURCE_DIR "${LIBARP_ROOT_DIR}/src")
set(LIBARP_LIBRARY "libarp_static")
set(LIBARP_INCLUDE_DIR "${LIBARP_ROOT_DIR}/include")
add_subdirectory("${LIBARP_ROOT_DIR}")
list(APPEND EXT_LIBS "${LIBARP_LIBRARY}")

set(SRC_DIR "${PROJECT_SOURCE_DIR}/src")
set(INC_DIR "${PROJECT_SOURCE_DIR}/include")

set(GENERATED_SRC_DIR "${SRC_DIR}/generated")
set(GENERATED_INC_DIR "${INC_DIR}/internal/generated")

# source files
file(GLOB_RECURSE C_FILES ${SRC_DIR}/*.c)
file(GLOB_RECURSE H_FILES ${SRC_DIR}/*.h)

set(DIST_DIR "${CMAKE_BINARY_DIR}/dist")

set(LIB_OUT_DIR "${DIST_DIR}/lib")

set(LIBRARY_OUTPUT_PATH "${LIB_OUT_DIR}")

add_executable("${PROJECT_NAME}" ${C_FILES} ${H_FILES})

target_include_directories("${PROJECT_NAME}" PUBLIC "${INC_DIR};${LIBARP_INCLUDE_DIR}")

target_link_libraries("${PROJECT_NAME}" "${EXT_LIBS}")

# set the C standard
set_target_properties("${PROJECT_NAME}" PROPERTIES C_STANDARD 11)
# enable PIC
set_target_properties("${PROJECT_NAME}" PROPERTIES POSITION_INDEPENDENT_CODE ON)
# export all symbols (required on Windows)
set_target_properties("${PROJECT_NAME}" PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS ON)
# set target name
set_target_properties("${PROJECT_NAME}" PROPERTIES OUTPUT_NAME "${PROJECT_NAME}")

if(MSVC)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /W4")
  set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} /Od")
  if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    if(MSVC_VERSION LESS 1911)
      target_link_options(${PROJECT_NAME} PUBLIC "/DEBUG:FULL")
    else()
      target_link_options(${PROJECT_NAME} PUBLIC "/DEBUG:FASTLINK")
    endif()
  endif()
  set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} /O2")
else()
  set(CMAKE_C_FLAGS "-Wall -Wextra -Winit-self -Wuninitialized \
                     -Wmissing-declarations -Winit-self -pedantic -pedantic-errors -std=c11")
  if(IS_TARGET_X86)
    SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -msse4.2")
  endif()
  set(CMAKE_C_FLAGS_DEBUG "-g")
  set(CMAKE_C_FLAGS_RELEASE "-O3")
endif()
