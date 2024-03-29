cmake_minimum_required(VERSION 3.12)
if(POLICY CMP0048)
  cmake_policy(SET CMP0048 NEW)
endif()

project(arptool VERSION 1.0.0
                LANGUAGES C)

if("${CMAKE_VS_WINDOWS_TARGET_PLATFORM_VERSION}" STREQUAL "10.0.17763.0")
  message(FATAL_ERROR "Compilation against Windows SDK version 10.0.17763 is broken due to a bug in the SDK headers. \
  Please use a different SDK version.")
endif()

set(ARPTOOL_VERSION_SUFFIX "-dev")

set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)
if("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")
  message(SEND_ERROR "In-source builds are not allowed.")
endif()

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
  set(DEF_USE_SYSTEM_ZLIB OFF)
else()
  set(DEF_USE_SYSTEM_ZLIB ON)
endif()

# this gets passed straight through to libarp
option(FEATURE_DEFLATE "Compile with support for DEFLATE compression" ON)

option(USE_SYSTEM_ZLIB "Use system-provided zlib library and headers" "${DEF_USE_SYSTEM_ZLIB}")

set(LIBARP_FEATURE_DEFLATE "${FEATURE_DEFLATE}" CACHE BOOL "")

set(CMAKE_C_OUTPUT_EXTENSION_REPLACE 1)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

set(EXT_LIBS "")

set(ARP_ROOT_DIR "${PROJECT_SOURCE_DIR}/libs/libarp")
set(ARP_SOURCE_DIR "${LIBARP_ROOT_DIR}/src")
set(ARP_LIBRARY "arp_static")
set(ARP_INCLUDE_DIR "${LIBARP_ROOT_DIR}/include")
add_subdirectory("${ARP_ROOT_DIR}")
list(APPEND EXT_LIBS "${ARP_LIBRARY}")

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

configure_file("${INC_DIR}/config.h.in" "${INC_DIR}/config.h")

target_include_directories("${PROJECT_NAME}" PUBLIC "${INC_DIR};${LIBARP_INCLUDE_DIR}")

target_link_libraries("${PROJECT_NAME}" "${EXT_LIBS}")

# set the C standard
set_target_properties("${PROJECT_NAME}" PROPERTIES C_STANDARD 11)
set_target_properties("${PROJECT_NAME}" PROPERTIES C_STANDARD_REQUIRED ON)
set_target_properties("${PROJECT_NAME}" PROPERTIES C_EXTENSIONS OFF)
# enable PIC
set_target_properties("${PROJECT_NAME}" PROPERTIES POSITION_INDEPENDENT_CODE ON)
# export all symbols (required on Windows)
set_target_properties("${PROJECT_NAME}" PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS ON)
# set target name
set_target_properties("${PROJECT_NAME}" PROPERTIES OUTPUT_NAME "${PROJECT_NAME}")

target_compile_definitions(${PROJECT_NAME} PUBLIC "$<$<CONFIG:DEBUG>:ARPTOOL_DEBUG>")

if(MSVC)
  add_compile_definitions("_CRT_SECURE_NO_WARNINGS" "_CRT_NONSTDC_NO_WARNINGS")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /W4 /wd4244 /wd4267")
  set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} /Od")
  if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    if(MSVC_VERSION LESS 1911)
      set_property(TARGET "${PROJECT_NAME}" APPEND PROPERTY LINK_FLAGS /DEBUG:FULL)
    else()
      set_property(TARGET "${PROJECT_NAME}" APPEND PROPERTY LINK_FLAGS /DEBUG)
    endif()
  endif()
  set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} /O2")
else()
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Werror -Wall -Wextra -Winit-self -Wuninitialized -Wmissing-declarations \
                     -Winit-self -Wconversion -Wno-error=conversion -Wno-error=sign-conversion \
                     -pedantic -pedantic-errors")
  if(IS_TARGET_X86)
    SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -msse4.2")
  endif()
  if(MINGW)
    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -ggdb")
  endif()

  set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -O2")
endif()
