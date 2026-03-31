cmake_minimum_required( VERSION 4.1.1 )

project( idTech5Unpack )

set( CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}" )
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}" )
set( CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake" )

set( CMAKE_CXX_STANDARD_REQUIRED ON )
set( CMAKE_CXX_STANDARD 20 )
set( CMAKE_CXX_EXTENSIONS OFF )

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_compile_definitions( $<$<CONFIG:Release>:BUILD_RELEASE> )
add_compile_definitions( $<$<CONFIG:Debug>:BUILD_DEBUG> )

set( EXTERN_DIR "${CMAKE_SOURCE_DIR}/ext" )
set( SOURCE_DIR "${CMAKE_SOURCE_DIR}/src" )

#--------------------------------------------------------------------------------------------------------------------------------
if( MSVC )
    add_compile_definitions( _CRT_SECURE_NO_WARNINGS )
    add_compile_options( /W4 /WX )
else()
    add_compile_options( -Wall -Wextra -Wpedantic -Werror )
endif()

add_compile_definitions( SOURCE_DIR="${SOURCE_DIR}" )

add_compile_definitions( $<$<CONFIG:Debug>:DEBUG_MEMORY> )

#--------------------------------------------------------------------------------------------------------------------------------
set( LIB_SOURCES    "${SOURCE_DIR}/lib/containers/str.cpp"
                    "${SOURCE_DIR}/lib/math/hash.cpp"
                    "${SOURCE_DIR}/lib/math/rand.cpp"
                    "${SOURCE_DIR}/lib/mem/memory.cpp"
                    "${SOURCE_DIR}/lib/precompiled.cpp" )
set( LIB_HEADERS    "${SOURCE_DIR}/lib/containers/linklist.h"
                    "${SOURCE_DIR}/lib/containers/list.h"
                    "${SOURCE_DIR}/lib/containers/map.h"
                    "${SOURCE_DIR}/lib/containers/ref.h"
                    "${SOURCE_DIR}/lib/containers/sort.h"
                    "${SOURCE_DIR}/lib/containers/span.h"
                    "${SOURCE_DIR}/lib/containers/staticlist.h"
                    "${SOURCE_DIR}/lib/containers/staticstr.h"
                    "${SOURCE_DIR}/lib/containers/str.h"
                    "${SOURCE_DIR}/lib/math/math.h"
                    "${SOURCE_DIR}/lib/math/rand.h"
                    "${SOURCE_DIR}/lib/mem/memory.h"
                    "${SOURCE_DIR}/lib/sys/defines.h"
                    "${SOURCE_DIR}/lib/sys/system.h"
                    "${SOURCE_DIR}/lib/va.h" )
if( WIN32 )
    list( APPEND LIB_SOURCES    "${SOURCE_DIR}/lib/sys/sys_win32.cpp" )
elseif( APPLE )
    list( APPEND LIB_SOURCES    "${SOURCE_DIR}/lib/sys/sys_macos.cpp" )
else()
    message( FATAL_ERROR "Unsupported system" )
endif()

#--------------------------------------------------------------------------------------------------------------------------------
set( PROJECT_SOURCES	"${SOURCE_DIR}/resource.cpp"
						"${SOURCE_DIR}/unpack.cpp" )
set( PROJECT_HEADERS	"${SOURCE_DIR}/resource.h"
						"${SOURCE_DIR}/unpack.h" )
if( WIN32 )
    list( APPEND PROJECT_SOURCES    "${SOURCE_DIR}/sys/sys_win32.cpp" )
elseif( APPLE )
    list( APPEND PROJECT_HEADERS    "${SOURCE_DIR}/sys/sys_macos.cpp" )
else()
    message( FATAL_ERROR "Unsupported system" )
endif()

set( PROJECT_NAME   "${CMAKE_PROJECT_NAME}" )

add_executable( ${PROJECT_NAME}	"${PROJECT_HEADERS}"
                                "${PROJECT_SOURCES}"
                                "${LIB_HEADERS}"
                                "${LIB_SOURCES}" )
target_include_directories( ${PROJECT_NAME} PUBLIC "${EXTERN_DIR}" "${SOURCE_DIR}" )
target_precompile_headers( ${PROJECT_NAME}  PUBLIC "${SOURCE_DIR}/lib/precompiled.h" )
if( APPLE )
    target_link_libraries( ${PROJECT_NAME}  "-framework CoreFoundation" )
endif()
