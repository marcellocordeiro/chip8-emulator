cmake_minimum_required(VERSION 3.9)

project(chip8-emulator VERSION 1.0.0)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_HOME_DIRECTORY}/bin)

######################
#    Dependencies    #
######################

include_directories("include")

find_package(SFML 2.5 COMPONENTS graphics audio REQUIRED)

######################
#    Source files    #
######################

file(GLOB SOURCE_FILES
    "src/*.cpp"
)

######################
#     Executable     #
######################

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

######################
#   Compiler flags   #
######################

set(GNU_FLAGS
    -Werror
    -Wall
    -Wextra
    -pedantic
    -Wuseless-cast
    -Wshadow
    -Wformat=2
    -Wdisabled-optimization
)

set(MSVC_FLAGS
    /WX
    /W4
    /permissive-
    /diagnostics:caret
    /Zi
    /wd4805
    /wd4267
    /wd4100
    /wd4201
)

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)

target_compile_options(${PROJECT_NAME} PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:${MSVC_FLAGS}>
    #$<$<BOOL:MSVC>:${MSVC_FLAGS}>
    $<$<CXX_COMPILER_ID:GNU>:${GNU_FLAGS}>
)

if (MSVC)
    string(REGEX REPLACE "/W3" "/W4" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
endif()

##################
# Link libraries #
##################

if(UNIX)
    if (CMAKE_BUILD_TYPE STREQUAL "Debug")
        target_compile_options(${PROJECT_NAME} PRIVATE -fsanitize=address,undefined)
        target_link_libraries(${PROJECT_NAME} asan ubsan)
    endif()
endif()

target_link_libraries(${PROJECT_NAME} sfml-graphics sfml-audio)