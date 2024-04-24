cmake_minimum_required(VERSION 3.5.1)

message(STATUS "\n## In directory: ${CMAKE_CURRENT_SOURCE_DIR}")

set(ENV{MYDIR} ${CMAKE_CURRENT_SOURCE_DIR})
set(ENV{MYDIR} )

set(SCRIPTS_DIR "" CACHE STRING "CMake scripts path")

project(my-project
    DESCRIPTION "My project"
)

message(STATUS "\n**********************************************************************************\n")
message(STATUS "\n## In directory: ${CMAKE_CURRENT_SOURCE_DIR}")

message("\n** Setting up ${PROJECT_NAME} **\n")

set(PLATFORM_NAME Windows)
set(OUTPUT_BASE_DIR ${CMAKE_SOURCE_DIR}/output/${PLATFORM_NAME})
set(CONFIG_DIR Debug)
set(COMPILER_DEFINITIONS )
set(COMPILER_OPTIONS_CXX )
set(LINKER_OPTIONS )
set(LINK_DIRECTORIES )
set(LINKER_LIBRARIES )

set(TARGET_NAME ${PROJECT_NAME})

set(PROJECT_COMPILER_DEFINITIONS_PRIVATE
    "PACKAGE_NAME=\"${PROJECT_NAME}\""
    ${COMPILER_DEFINITIONS}
    )

set(PROJECT_COMPILER_DEFINITIONS_PUBLIC
    )

set(PROJECT_COMPILER_OPTIONS_PRIVATE
    ${COMPILER_OPTIONS_CXX}
    /wd4191 /wd4706)

set(PROJECT_COMPILER_OPTIONS_PUBLIC
    )

set(PROJECT_INCLUDE_DIRS_PRIVATE
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/src
    )

set(PROJECT_INCLUDE_DIRS_PUBLIC
    )

set(PROJECT_LINK_OPTIONS
    ${LINKER_OPTIONS}
    )

set(PROJECT_DEPENDENCIES
    )

set(PROJECT_LIBS
    ${LINKER_LIBRARIES}
    ${PROJECT_DEPENDENCIES}
    )

set(PROJECT_SOURCES 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
    )

set(PROJECT_INCLUDES_PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include/getopt.h
    )

set(PROJECT_INCLUDES_PUBLIC
    )

set(ENV{myvar} "Hello world")

unset(SCRIPTS_DIR CACHE)
unset(ENV{MYDIR} )
