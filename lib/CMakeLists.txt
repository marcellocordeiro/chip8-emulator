cmake_minimum_required(VERSION 3.15)

project(chip8-emulator-lib)

# Setting up the target

add_library(chip8-emulator-lib STATIC)

target_sources(chip8-emulator-lib PRIVATE
  "src/log.cpp"
  "src/random.cpp"
  "src/system_utils.cpp"
  "src/timer.cpp"
)

target_include_directories(chip8-emulator-lib SYSTEM PUBLIC "include")

# Compiler settings

target_compile_features(chip8-emulator-lib PRIVATE cxx_std_17)
