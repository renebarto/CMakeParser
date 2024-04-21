project(test-platform
    VERSION ${MSI_NUMBER}
    DESCRIPTION "Test platform includes and dependencies for Google Test / Mock"
    LANGUAGES CXX)

message(STATUS "\n**********************************************************************************\n")
message(STATUS "\n## In directory: ${CMAKE_CURRENT_SOURCE_DIR}")

message("\n** Setting up ${PROJECT_NAME} **\n")

set(PROJECT_BUILD_REFERENCE "\"${PROJECT_VERSION}\"")

set(PROJECT_COMPILER_DEFINITIONS_INTERFACE
    ${COMPILER_DEFINITIONS}
    )

set(PROJECT_COMPILER_OPTIONS_INTERFACE
    ${COMPILER_OPTIONS_CXX}
    )

set(PROJECT_INCLUDE_DIRS_INTERFACE
    ${PROJECT_SOURCE_DIR}/include
    )

set(PROJECT_DEPENDENCIES
    gtest
    gmock
    )

set(PROJECT_LIBS
    ${LINKER_LIBRARIES}
    ${PROJECT_DEPENDENCIES}
    )

set(PROJECT_INCLUDES_INTERFACE
    include/test-platform/GoogleMock.h
    include/test-platform/GoogleTest.h
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
    display_list("Defines - interface               : " ${PROJECT_COMPILER_DEFINITIONS_INTERFACE} )
    display_list("Compiler options - interface      : " ${PROJECT_COMPILER_DEFINITIONS_INTERFACE} )
    display_list("Include dirs - interface          : " ${PROJECT_INCLUDE_DIRS_INTERFACE} )
    display_list("Linker options                    : " ${PROJECT_LINK_OPTIONS} )
    display_list("Dependencies                      : " ${PROJECT_DEPENDENCIES} )
    display_list("Link libs                         : " ${PROJECT_LIBS} )
    display_list("Source files                      : " ${PROJECT_SOURCES} )
    display_list("Include files - interface         : " ${PROJECT_INCLUDES_INTERFACE} )
endif()

add_library(${PROJECT_NAME} INTERFACE)
target_link_libraries(${PROJECT_NAME} INTERFACE ${PROJECT_LIBS})
target_include_directories(${PROJECT_NAME} INTERFACE ${PROJECT_INCLUDE_DIRS_INTERFACE})
target_compile_definitions(${PROJECT_NAME} INTERFACE ${PROJECT_COMPILER_DEFINITIONS_INTERFACE})
if (PROJECT_BUILD_REFERENCE)
    target_compile_definitions(${PROJECT_NAME} INTERFACE BUILD_REFERENCE=${PROJECT_BUILD_REFERENCE})
endif()
target_compile_options(${PROJECT_NAME} INTERFACE ${PROJECT_COMPILER_OPTIONS_INTERFACE})

show_target_properties(${PROJECT_NAME})
