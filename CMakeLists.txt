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

find_package(SFML COMPONENTS window graphics audio REQUIRED)

######################
#     Executable     #
######################

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

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

target_link_libraries(${PROJECT_NAME} sfml-window sfml-graphics sfml-audio)