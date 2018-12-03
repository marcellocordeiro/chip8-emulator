cmake_minimum_required(VERSION 3.9)

project(sfml-builder)

######################
#        SFML        #
######################

find_package(Git REQUIRED)

include(ExternalProject)

set(SFML_INSTALL_DIR ${CMAKE_HOME_DIRECTORY}/SFML)

ExternalProject_Add(
    SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.5.1
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${SFML_INSTALL_DIR} -DBUILD_SHARED_LIBS=ON
)

######################
#        SDL2        #
######################

find_package(Git REQUIRED)

include(ExternalProject)

set(SDL2_INSTALL_DIR ${CMAKE_HOME_DIRECTORY}/SDL2)

ExternalProject_Add(
    SDL2
    GIT_REPOSITORY https://github.com/spurious/SDL-mirror.git
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${SDL2_INSTALL_DIR} -DCMAKE_BUILD_TYPE=Release
)
