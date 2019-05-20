cmake_minimum_required(VERSION 3.9)

project(chip8-emulator VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_HOME_DIRECTORY}/bin)

######################
#      Compiler      #
######################

set(GCC_CLANG FALSE)
set(CLANGCL FALSE)

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    if(NOT MSVC)
        set(GCC_CLANG TRUE)
    elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
        set(CLANGCL TRUE)
    endif()
endif()

if(NOT GCC_CLANG AND NOT CLANGCL AND NOT MSVC)
    message(FATAL_ERROR "Compiler not supported")
endif()

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

if(EXISTS ${CMAKE_HOME_DIRECTORY}/ext/SFML)
	set(SFML_DIR ${CMAKE_HOME_DIRECTORY}/ext/SFML/lib/cmake/SFML)
endif()

# set(SFML_STATIC_LIBRARIES TRUE)

find_package(SFML 2.5 COMPONENTS graphics audio REQUIRED)

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
    if(GCC_CLANG)
        target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -pedantic)
        target_compile_options(${PROJECT_NAME} PRIVATE -Werror)
    endif()
endif()

if(MSVC OR CLANGCL)
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


    if(GCC_CLANG)
        target_link_libraries(${PROJECT_NAME} stdc++fs)
    endif()
endif()

target_link_libraries(${PROJECT_NAME} sfml-graphics sfml-audio)

##################
#    Install     #
##################

set(CMAKE_INSTALL_PREFIX "${CMAKE_HOME_DIRECTORY}")

if(WIN32)
    install(FILES "${CMAKE_HOME_DIRECTORY}/ext/SFML/bin/openal32.dll" DESTINATION bin)
    install(FILES "${CMAKE_HOME_DIRECTORY}/ext/SFML/bin/sfml-audio-2.dll" DESTINATION bin)
    install(FILES "${CMAKE_HOME_DIRECTORY}/ext/SFML/bin/sfml-graphics-2.dll" DESTINATION bin)
    install(FILES "${CMAKE_HOME_DIRECTORY}/ext/SFML/bin/sfml-system-2.dll" DESTINATION bin)
    install(FILES "${CMAKE_HOME_DIRECTORY}/ext/SFML/bin/sfml-window-2.dll" DESTINATION bin)
endif()

install(TARGETS ${PROJECT_NAME} DESTINATION bin)