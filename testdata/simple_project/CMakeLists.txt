cmake_minimum_required(VERSION 3.5.1)

project(main-project
    VERSION 4.3.2.1
    DESCRIPTION "This is the main project"
    LANGUAGES CXX
)

project(mylib
    VERSION 4.3.2.1
    DESCRIPTION "This is the library project"
    LANGUAGES CXX
)

add_library(${PROJECT_NAME} STATIC src/lib.cpp include/lib.h)
target_include_directories(${PROJECT_NAME} PRIVATE )
target_include_directories(${PROJECT_NAME} PUBLIC  include include2
    include3 )
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)

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
set(PROJECT_INCLUDES_PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include/private_header.h
    )

set(PROJECT_INCLUDES_PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include/public_header.h
    )

set(PROJECT_SOURCES 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
    )

project(myexe
    VERSION 4.3.2.1
    DESCRIPTION "This is the executable project"
    LANGUAGES CXX
)

add_executable(${PROJECT_NAME} ${PROJECT_SOURCES} ${PROJECT_INCLUDES_PUBLIC} ${PROJECT_INCLUDES_PRIVATE})
target_link_libraries(${PROJECT_NAME} mylib)
target_include_directories(${PROJECT_NAME} PRIVATE include)
target_include_directories(${PROJECT_NAME} PUBLIC  )
target_compile_definitions(${PROJECT_NAME} PRIVATE ${PROJECT_COMPILER_DEFINITIONS_PRIVATE})
target_compile_definitions(${PROJECT_NAME} PUBLIC  ${PROJECT_COMPILER_DEFINITIONS_PUBLIC})
target_compile_options(${PROJECT_NAME} PRIVATE ${PROJECT_COMPILER_OPTIONS_PRIVATE})
target_compile_options(${PROJECT_NAME} PUBLIC  ${PROJECT_COMPILER_OPTIONS_PUBLIC})
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)
