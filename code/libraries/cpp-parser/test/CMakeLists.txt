project(cpp-parser-test
    VERSION ${MSI_NUMBER}
    DESCRIPTION "CPP parser test"
    LANGUAGES CXX)

message(STATUS "\n**********************************************************************************\n")
message(STATUS "\n## In directory: ${CMAKE_CURRENT_SOURCE_DIR}")

message("\n** Setting up ${PROJECT_NAME} **\n")

set(TARGET_NAME ${PROJECT_NAME})

set(PROJECT_BUILD_REFERENCE "\"${PROJECT_VERSION}\"")

set(PROJECT_COMPILER_DEFINITIONS_PRIVATE
    "PACKAGE_NAME=\"${PROJECT_NAME}\""
    ${COMPILER_DEFINITIONS}
    )

set(PROJECT_COMPILER_DEFINITIONS_PUBLIC
    )

set(PROJECT_COMPILER_OPTIONS_PRIVATE
    ${COMPILER_OPTIONS_CXX}
    )

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
    cpp-parser
    test-platform
    )

set(PROJECT_LIBS
    ${LINKER_LIBRARIES}
    ${PROJECT_DEPENDENCIES}
    )

set(PROJECT_SOURCES 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ParserTest.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/LexerTest.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
    )
set(PROJECT_SOURCES_${PROJECT_NAME}
    ${PROJECT_SOURCES}
    CACHE STRING "${PROJECT_NAME}" FORCE)

set(PROJECT_INCLUDES_PRIVATE
    )

set(PROJECT_INCLUDES_PUBLIC
    )

if (CMAKE_VERBOSE_MAKEFILE)
    display_list("Package                           : " ${PROJECT_NAME} )
    display_list("Package description               : " ${PROJECT_DESCRIPTION} )
    display_list("Package version major             : " ${PROJECT_VERSION_MAJOR} )
    display_list("Package version minor             : " ${PROJECT_VERSION_MINOR} )
    display_list("Package version level             : " ${PROJECT_VERSION_PATCH} )
    display_list("Package version build             : " ${PROJECT_VERSION_TWEAK} )
    display_list("Package version                   : " ${PROJECT_VERSION} )
    display_list("Build reference                   : " ${PROJECT_BUILD_REFERENCE} )
    display_list("Defines - public                  : " ${PROJECT_COMPILER_DEFINITIONS_PUBLIC} )
    display_list("Defines - private                 : " ${PROJECT_COMPILER_DEFINITIONS_PRIVATE} )
    display_list("Compiler options - public         : " ${PROJECT_COMPILER_OPTIONS_PUBLIC} )
    display_list("Compiler options - private        : " ${PROJECT_COMPILER_OPTIONS_PRIVATE} )
    display_list("Include dirs - public             : " ${PROJECT_INCLUDE_DIRS_PUBLIC} )
    display_list("Include dirs - private            : " ${PROJECT_INCLUDE_DIRS_PRIVATE} )
    display_list("Linker options                    : " ${PROJECT_LINK_OPTIONS} )
    display_list("Dependencies                      : " ${PROJECT_DEPENDENCIES} )
    display_list("Link libs                         : " ${PROJECT_LIBS} )
    display_list("Source files                      : " ${PROJECT_SOURCES} )
    display_list("Include files - public            : " ${PROJECT_INCLUDES_PUBLIC} )
    display_list("Include files - private           : " ${PROJECT_INCLUDES_PRIVATE} )
endif()

link_directories(${LINK_DIRECTORIES})
add_executable(${PROJECT_NAME} ${PROJECT_SOURCES} ${PROJECT_INCLUDES_PUBLIC} ${PROJECT_INCLUDES_PRIVATE})
target_link_libraries(${PROJECT_NAME} ${PROJECT_LIBS})
target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_INCLUDE_DIRS_PRIVATE})
target_include_directories(${PROJECT_NAME} PUBLIC  ${PROJECT_INCLUDE_DIRS_PUBLIC})
target_compile_definitions(${PROJECT_NAME} PRIVATE ${PROJECT_COMPILER_DEFINITIONS_PRIVATE})
target_compile_definitions(${PROJECT_NAME} PUBLIC  ${PROJECT_COMPILER_DEFINITIONS_PUBLIC})
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD ${SUPPORTED_CPP_STANDARD})
if (PROJECT_BUILD_REFERENCE)
    target_compile_definitions(${PROJECT_NAME} PRIVATE BUILD_REFERENCE=${PROJECT_BUILD_REFERENCE})
endif()
target_compile_options(${PROJECT_NAME} PRIVATE ${PROJECT_COMPILER_OPTIONS_PRIVATE})
target_compile_options(${PROJECT_NAME} PUBLIC  ${PROJECT_COMPILER_OPTIONS_PUBLIC})

list_to_string(PROJECT_LINK_OPTIONS PROJECT_LINK_OPTIONS_STRING)
if (NOT "${PROJECT_LINK_OPTIONS_STRING}" STREQUAL "")
    set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "${PROJECT_LINK_OPTIONS_STRING}")
endif()

set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH})
set_target_properties(${PROJECT_NAME} PROPERTIES SOVERSION ${PROJECT_VERSION_MAJOR})
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME ${TARGET_NAME})
set_target_properties(${PROJECT_NAME} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${OUTPUT_BASE_DIR}/${CONFIG_DIR}/lib)
set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_BASE_DIR}/${CONFIG_DIR}/bin)

show_target_properties(${PROJECT_NAME})
