cmake_minimum_required(VERSION 3.9)

project(chip8-emulator VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_HOME_DIRECTORY}/bin)

######################
#    Dependencies    #
######################

find_package(SFML 2.5 COMPONENTS graphics audio REQUIRED)

######################
#    Source files    #
######################

file(GLOB SOURCE_FILES
    "src/*.h"
    "src/*.cpp"
)

######################
#     Executable     #
######################

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

######################
#   Compiler flags   #
######################

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)

target_compile_options(
    ${PROJECT_NAME}
    PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/W4 /permissive->
    $<$<CXX_COMPILER_ID:GNU>:-Wall -Wextra -pedantic>
)
string(REGEX REPLACE "/W[0-9]" "/W4" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})

target_compile_options(
    ${PROJECT_NAME}
    PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/WX>
    $<$<CXX_COMPILER_ID:GNU>:-Werror>
)

target_compile_options(
    ${PROJECT_NAME}
    PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/diagnostics:caret /Zi>
)

target_compile_options(
    ${PROJECT_NAME}
    PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/Zc:rvalueCast /Zc:referenceBinding /Zc:strictStrings>
)

##################
# Link libraries #
##################

if(UNIX)
    if (CMAKE_BUILD_TYPE STREQUAL "Debug")
        target_compile_options(${PROJECT_NAME} PRIVATE -fsanitize=undefined,address)
        target_link_libraries(${PROJECT_NAME} asan ubsan)
    endif()
    
    target_link_libraries(${PROJECT_NAME} stdc++fs)
endif()

target_link_libraries(${PROJECT_NAME} sfml-graphics sfml-audio)