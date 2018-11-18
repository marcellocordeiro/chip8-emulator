cmake_minimum_required(VERSION 3.9)

project(chip8-emulator)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_HOME_DIRECTORY}/build)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_HOME_DIRECTORY}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_HOME_DIRECTORY}/lib)

if (GNU OR Clang)
    set(CMAKE_CXX_STANDARD 17)

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror -pedantic-errors")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m64")

    if (UNIX)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=undefined,address")
    endif()
endif()

if (MSVC)
    set(CMAKE_CXX_STANDARD 17)

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4 /WX")
    # set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /permissive-")
    # set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Zc:referenceBinding /Zc:rvalueCast /Zc:stringStrings")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /diagnostics:caret")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Zi")
endif()

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
include_directories(${SFML_INSTALL_DIR}/include)
link_directories(${SFML_INSTALL_DIR}/lib)

######################

file(GLOB SOURCE_FILES
    "src/*.h"
    "src/*.cpp"
)
add_executable(${PROJECT_NAME} ${SOURCE_FILES})

######################

add_dependencies(${PROJECT_NAME} SFML)

# set(SFML_DIR "build/external/lib/cmake/SFML")
# find_package(SFML COMPONENTS window graphics audio REQUIRED)
target_link_libraries(${PROJECT_NAME} sfml-window sfml-graphics sfml-audio sfml-system)

if (GNU OR Clang)
    target_link_libraries(${PROJECT_NAME} stdc++fs)
endif()