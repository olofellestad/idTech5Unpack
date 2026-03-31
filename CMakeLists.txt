cmake_minimum_required( VERSION 4.1.1 )

project( idTech5Unpack )

set( CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}" )
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}" )
set( CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake" )

set( CMAKE_CXX_STANDARD_REQUIRED ON )
set( CMAKE_CXX_STANDARD 20 )
set( CMAKE_CXX_EXTENSIONS OFF )

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if( MSVC )
	add_compile_definitions( _CRT_SECURE_NO_WARNINGS )
	add_compile_options( /W4 /WX )
else()
	add_compile_options( -Wall -Wextra -Wpedantic -Werror )
endif()

add_compile_definitions( $<$<CONFIG:Release>:BUILD_RELEASE> )
add_compile_definitions( $<$<CONFIG:Debug>:BUILD_DEBUG> )

set( EXTERN_DIR "${CMAKE_SOURCE_DIR}/ext" )
set( SOURCE_DIR "${CMAKE_SOURCE_DIR}/src" )

#--------------------------------------------------------------------------------------------------------------------------------

#--------------------------------------------------------------------------------------------------------------------------------
set( PROJECT_SOURCES	"${SOURCE_DIR}/main.cpp"
						"${SOURCE_DIR}/resource.cpp"
						"${SOURCE_DIR}/unpack.cpp" )
set( PROJECT_HEADERS	"${SOURCE_DIR}/resource.h"
						"${SOURCE_DIR}/unpack.h" )
set( PROJECT_NAME		"${CMAKE_PROJECT_NAME}" )

add_executable( ${PROJECT_NAME}	"${PROJECT_HEADERS}"
								"${PROJECT_SOURCES}" )