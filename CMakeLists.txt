cmake_minimum_required(VERSION 3.5.1)

message(STATUS "\n## In directory: ${CMAKE_CURRENT_SOURCE_DIR}")

function(dump_all_vars)
    get_cmake_property(_variableNames VARIABLES)
    foreach (_variableName ${_variableNames})
        message(STATUS "${_variableName}=${${_variableName}}")
    endforeach()
endfunction()

dump_all_vars()

set(SCRIPTS_DIR "" CACHE STRING "CMake scripts path")

if ("${SCRIPTS_DIR}" STREQUAL "")
    set(SCRIPTS_DIR "${CMAKE_SOURCE_DIR}/cmake")
endif()

list(APPEND CMAKE_MODULE_PATH ${SCRIPTS_DIR})

include(functions)
include(macros)
option(VERBOSE_BUILD "Verbose build" ON)

message(STATUS "\n** Setting up project **\n--")

parse_msi(MSI_NUMBER MSI_VERSION_MAJOR MSI_VERSION_MINOR MSI_VERSION_LEVEL MSI_VERSION_BUILD)
message(STATUS "MSI version: ${MSI_VERSION_MAJOR}.${MSI_VERSION_MINOR}.${MSI_VERSION_LEVEL}.${MSI_VERSION_BUILD}")

message(STATUS "\n** Setting up toolchain **\n--")

project(CPP-Parser
    VERSION ${MSI_NUMBER}
    DESCRIPTION "This is the project for cpp-parser"
)

message(STATUS "")

set(PLATFORM_WINDOWS TRUE)
message(STATUS "Building for Windows with MSVC")
set(PLATFORM_NAME windows)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_COLOR_MAKEFILE   OFF)

set(CMAKE_BUILD_WITH_INSTALL_RPATH ON)

if (VERBOSE_BUILD)
    set(CMAKE_VERBOSE_MAKEFILE ON)
else()
    set(CMAKE_VERBOSE_MAKEFILE OFF)
endif()

message(STATUS "")
message(STATUS "Verbose build:       ${CMAKE_VERBOSE_MAKEFILE}")

set(OUTPUT_BASE_DIR ${CMAKE_SOURCE_DIR}/output/${PLATFORM_NAME})
set(TEST_DATA_DIR ${CMAKE_SOURCE_DIR}/testdata)

# Enforce CMake to use /MT and /MTd
set(CompilerFlags
    CMAKE_CXX_FLAGS
    CMAKE_CXX_FLAGS_DEBUG
    CMAKE_CXX_FLAGS_RELEASE
    CMAKE_C_FLAGS
    CMAKE_C_FLAGS_DEBUG
    CMAKE_C_FLAGS_RELEASE
    )
foreach(CompilerFlag ${CompilerFlags})
    string(REPLACE "/MD" "/MT" ${CompilerFlag} "${${CompilerFlag}}")
endforeach()

set(SUPPORTED_CPP_STANDARD 17)

# Important! do not keep warning 4251 4275 ignored!
# Possibly switch warning 4505 back on
set(FLAGS_CXX
    /Wall /wd4061 /wd4239 /wd4251 /wd4275 /wd4435 /wd4505 /wd4514 /wd4548 /wd4571 /wd4592 /wd4625 /wd4626 /wd4628 /wd4710 /wd4711 /wd4774 /wd4668 /wd5045
    /wd4820 /wd5026 /wd5027 /WX- /EHsc /Gd /GR /sdl- /Zc:wchar_t /Zc:inline /fp:precise /bigobj /std:c++${SUPPORTED_CPP_STANDARD})
set(FLAGS_CXX_DEBUG /Od /Gm- /Zi /RTC1 /MTd)
set(FLAGS_CXX_RELEASE /Ox /GL /GS /Gy /Oi /MT)
set(FLAGS_CXX_MINSIZEREL /O1 /GL /GS /Gy /Oi /MT)
set(FLAGS_CXX_RELWITHDEBINFO /O2 /GL /GS /Gy /Oi /Zi /MT)

set(FLAGS_C
    /Wall /wd4061 /wd4251 /wd4275 /wd4505 /wd4514 /wd4548 /wd4571 /wd4625 /wd4626 /wd4710 /wd4774
    /wd4820 /wd5026 /wd5027 /WX- /EHsc /Gd /GR /sdl- /Zc:wchar_t /Zc:inline /fp:precise)
set(FLAGS_C_DEBUG /Od /Gm- /ZI /RTC1 /MTd)
set(FLAGS_C_RELEASE /Ox /GL /GS /Gy /Oi /MT)
set(FLAGS_C_MINSIZEREL /O1 /GL /GS /Gy /Oi /MT)
set(FLAGS_C_RELWITHDEBINFO /O2 /GL /GS /Gy /Oi /Zi /MT)

set(DEFINES _UNICODE UNICODE _CRT_SECURE_NO_WARNINGS _SCL_SECURE_NO_WARNINGS _WINSOCK_DEPRECATED_NO_WARNINGS WINVER=0x0A00 _WIN32_WINNT=0x0A00 PLATFORM_WINDOWS TEST_DATA_DIR="${TEST_DATA_DIR}")
set(DEFINES_DEBUG _DEBUG)
set(DEFINES_RELEASE NDEBUG)
set(DEFINES_MINSIZEREL NDEBUG)
set(DEFINES_RELWITHDEBINFO NDEBUG)

set(LINK_FLAGS /ignore:4217)
list(APPEND LINK_LIBRARIES )
list(APPEND LINK_DIRECTORIES )

# Hack to force linker to target X64 machine when needed.
if (${CMAKE_SIZEOF_VOID_P} EQUAL "8")
    set(DEFINES ${DEFINES} _AMD64_)
else ()
    set(DEFINES ${DEFINES} _X86_)
endif ()

if("${CMAKE_BUILD_TYPE}" STREQUAL "")
    set(CMAKE_BUILD_TYPE Debug)
endif()

set_config_dir()

message(STATUS "\n-- Find threading library\n--")
set(CMAKE_USE_WIN32_THREADS_INIT TRUE)
find_package(Threads REQUIRED)
if (Threads_FOUND)
    message(STATUS "CMAKE_THREAD_LIBS_INIT          = ${CMAKE_THREAD_LIBS_INIT}")
    message(STATUS "CMAKE_USE_WIN32_THREADS_INIT    = ${CMAKE_USE_WIN32_THREADS_INIT}")
    message(STATUS "CMAKE_USE_PTHREADS_INIT         = ${CMAKE_USE_PTHREADS_INIT}")
endif()

convert_default_options()

message(STATUS "C++ compiler version:    ${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS "C compiler version:      ${CMAKE_C_COMPILER_VERSION}")
message(STATUS "C++ supported standard:  ${SUPPORTED_CPP_STANDARD}")
message(STATUS "CMAKE_GENERATOR_TOOLSET: ${CMAKE_GENERATOR_TOOLSET}")

message(STATUS "\n** Setup google test **")
# to prevent a warning with google test, set the default value for policy CMP0048
message(STATUS "\n## In directory: ${CMAKE_SOURCE_DIR}/googletest-release-1.10.0")
set( CMAKE_POLICY_DEFAULT_CMP0048 NEW )
add_subdirectory(${CMAKE_SOURCE_DIR}/googletest-release-1.10.0 gtest_binary_dir)

add_subdirectory(code)
