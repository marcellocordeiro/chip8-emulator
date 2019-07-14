cmake_minimum_required(VERSION 3.14)

project(chip8-emulator VERSION 1.0.0)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_HOME_DIRECTORY}/bin)

######################
#    Dependencies    #
######################

find_package(SFML 2.5 COMPONENTS graphics audio REQUIRED)

add_subdirectory("lib")

######################
#    Source files    #
######################

include_directories("include")

file(GLOB SOURCE_FILES
    "src/*.cpp"
)

######################
#     Executable     #
######################

add_executable(chip8-emulator ${SOURCE_FILES})

######################
#   Compiler flags   #
######################

set(GNU_FLAGS
    -Werror
    -Wall
    -Wextra
    -pedantic
)

set(MSVC_FLAGS
    /WX
    /W4
    /permissive-
    /diagnostics:caret
)

set_target_properties(
    chip8-emulator
    PROPERTIES CXX_STANDARD 17
               CXX_STANDARD_REQUIRED YES
               CXX_EXTENSIONS NO
)

if(UNIX)
    target_compile_options(chip8-emulator PRIVATE ${GNU_FLAGS})
elseif(MSVC)
    target_compile_options(chip8-emulator PRIVATE ${MSVC_FLAGS})

    # Remove for CMake 3.15 (CMP0092)
    string(REGEX REPLACE "/W3" "/W4" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
endif()

##################
# Link libraries #
##################

target_link_libraries(chip8-emulator PRIVATE chip8-emulator-lib)
target_link_libraries(chip8-emulator PRIVATE sfml-graphics sfml-audio)
