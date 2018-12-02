cmake_minimum_required(VERSION 3.9)

project(chip8-emulator VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_HOME_DIRECTORY}/build)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_HOME_DIRECTORY}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_HOME_DIRECTORY}/lib)

######################
#    Source files    #
######################

file(GLOB SOURCE_FILES
    "src/*.h"
    "src/*.cpp"
)

######################
#        SFML        #
######################
find_package(Git REQUIRED)

include(ExternalProject)

set(SFML_INSTALL_DIR ${CMAKE_BINARY_DIR}/external/SFML)

ExternalProject_Add(
    SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.5.1
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${SFML_INSTALL_DIR}
)

# ExternalProject_Get_Property(SFML SFML_INSTALL_DIR)
# set(SFML_DIR "build/external/lib/cmake/SFML")
# find_package(SFML COMPONENTS window graphics audio REQUIRED)

include_directories(${SFML_INSTALL_DIR}/include)
link_directories(${SFML_INSTALL_DIR}/lib)

######################
#     Executable     #
######################

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

add_dependencies(${PROJECT_NAME} SFML)

######################
#   Compiler flags   #
######################

set_target_properties(
    ${PROJECT_NAME}
    PROPERTIES CXX_STANDARD 17
               CXX_STANDARD_REQUIRED YES
               CXX_EXTENSIONS NO
)

if(UNIX)
    if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" OR
       "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
        target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -pedantic)
        target_compile_options(${PROJECT_NAME} PRIVATE -Werror)
    endif()
endif()

if(MSVC)
    string(REGEX REPLACE "/W[0-9]" "/W4" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
    target_compile_options(${PROJECT_NAME} PRIVATE /WX)
    target_compile_options(${PROJECT_NAME} PRIVATE /permissive-)

    target_compile_options(${PROJECT_NAME} PRIVATE /diagnostics:caret)
    target_compile_options(${PROJECT_NAME} PRIVATE /Zi)

    target_compile_options(${PROJECT_NAME} PRIVATE /Zc:rvalueCast)
    # target_compile_options(${PROJECT_NAME} PRIVATE /Zc:referenceBinding)
    # target_compile_options(${PROJECT_NAME} PRIVATE /Zc:stringStrings)
endif()

##################
# Link libraries #
##################

if(UNIX)
    if (CMAKE_BUILD_TYPE STREQUAL "Debug")
        target_compile_options(${PROJECT_NAME} PRIVATE -fsanitize=undefined,address)
        target_link_libraries(${PROJECT_NAME} asan ubsan)
    endif()


    if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" OR
       "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
        target_link_libraries(${PROJECT_NAME} stdc++fs)
    endif()
endif()

target_link_libraries(${PROJECT_NAME} sfml-window sfml-graphics sfml-audio sfml-system)