cmake_minimum_required(VERSION 3.14)

project(chip8-emulator-lib)

set(SOURCE_FILES
    "src/log.cpp"
)

add_library(chip8-emulator-lib STATIC ${SOURCE_FILES})

target_compile_features(chip8-emulator-lib PRIVATE cxx_std_17)

target_include_directories(chip8-emulator-lib PUBLIC "include")
