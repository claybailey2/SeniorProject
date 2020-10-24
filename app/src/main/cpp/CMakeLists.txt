# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html
cmake_minimum_required(VERSION 3.4.1)
project(SeniorProject)

#locate amidi api in ndk
# TODO: make relative path
set(amidi-lib /Users/claybailey/Library/Android/sdk/ndk/21.3.6528147/sysroot/usr/)
find_library(amidi-lib amidi)

# Set the path to the Oboe directory
set (OBOE_DIR /Users/claybailey/Library/Android/oboe)
add_subdirectory(${OBOE_DIR} ./oboe)
include_directories(${OBOE_DIR}/include)

# TODO: import from repo
# find_package (oboe REQUIRED CONFIG)

add_library(native-lib SHARED
        native-lib.cpp
        AudioEngine.cpp
        #AppMidiManager.cpp
        )

#target_link_libraries(native-lib oboe::oboe)
target_link_libraries(native-lib PRIVATE oboe ${amidi-lib})

target_compile_options(native-lib
        PRIVATE
        -std=c++14
        -Wall
        -Wextra-semi
        -Wshadow
        -Wshadow-field
        -Ofast
        -g
        "$<$<CONFIG:DEBUG>:-Werror>")