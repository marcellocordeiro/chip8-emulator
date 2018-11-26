cmake_minimum_required(VERSION 3.9)

project(chip8-emulator)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_HOME_DIRECTORY}/build)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_HOME_DIRECTORY}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_HOME_DIRECTORY}/lib)

######################

file(GLOB SOURCE_FILES
    "src/*.h"
    "src/*.cpp"
)

######################
######## SFML ########
######################

set(SFML_INSTALL_DIR ${CMAKE_BINARY_DIR}/external/SFML)

include(ExternalProject)
find_package(Git REQUIRED)

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

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

add_dependencies(${PROJECT_NAME} SFML)
target_link_libraries(${PROJECT_NAME} sfml-window sfml-graphics sfml-audio sfml-system)

######################

if(GNU OR Clang)
    target_link_libraries(${PROJECT_NAME} stdc++fs)

    target_compile_options(${PROJECT_NAME} PRIVATE -std=c++17)

    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Werror -pedantic-errors)
    target_compile_options(${PROJECT_NAME} PRIVATE -m64)

    if(UNIX)
        target_compile_options(${PROJECT_NAME} PRIVATE -fsanitize=undefined,address)
    endif()
endif()

if(MSVC)
    # target_compile_options(${PROJECT_NAME} PRIVATE /std:c++latest)
    target_compile_options(${PROJECT_NAME} PRIVATE /std:c++17)

    string(REGEX REPLACE "/W[0-9]" "/W4" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
    target_compile_options(${PROJECT_NAME} PRIVATE /WX)
    target_compile_options(${PROJECT_NAME} PRIVATE /permissive-)

    target_compile_options(${PROJECT_NAME} PRIVATE /diagnostics:caret)
    target_compile_options(${PROJECT_NAME} PRIVATE /Zi)

    target_compile_options(${PROJECT_NAME} PRIVATE /Zc:rvalueCast)
    # target_compile_options(${PROJECT_NAME} PRIVATE /Zc:referenceBinding)
    # target_compile_options(${PROJECT_NAME} PRIVATE /Zc:stringStrings)
endif()