# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html
cmake_minimum_required(VERSION 3.4.1)
project(SeniorProject)

#locate amidi api in ndk
set(ANDROID_NDK /Users/claybailey/Library/Android/sdk/ndk/21.3.6528147/) # set to local NDK
find_library(amidi REQUIRED)
find_library(log REQUIRED)

#get oboe from repository
find_package (oboe REQUIRED CONFIG)

add_library(native-lib SHARED
        native-lib.cpp
        AudioEngine.cpp
        Composer.cpp
        )

target_link_libraries(native-lib oboe::oboe amidi log)

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