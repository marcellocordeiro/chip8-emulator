cmake_minimum_required(VERSION 3.15)

project(chip8-emulator)

# Setting up the target

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")

file(GLOB SOURCE_FILES
  "src/*.cpp"
)

add_executable(chip8-emulator ${SOURCE_FILES})

target_include_directories(chip8-emulator PRIVATE "include")

# Dependencies

add_subdirectory("lib")
target_link_libraries(chip8-emulator PRIVATE chip8-emulator-lib)

find_package(SFML 2.5 COMPONENTS graphics audio REQUIRED)
target_link_libraries(chip8-emulator PRIVATE sfml-graphics sfml-audio)

# Compiler settings

set_target_properties(chip8-emulator
  PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)

target_compile_options(chip8-emulator
  PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/W4 /permissive- /diagnostics:caret>
    $<$<CXX_COMPILER_ID:GNU,Clang>:-Wall -Wextra -pedantic -Wuseless-cast -Wshadow -Wformat=2 -Wdisabled-optimization>
)
