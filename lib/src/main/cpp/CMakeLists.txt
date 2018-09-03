# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.4.1)

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.

# 解决 'asm/types.h' file not found
if (${ANDROID_ABI} STREQUAL "arm64-v8a")
    include_directories(${ANDROID_SYSROOT}/usr/include/aarch64-linux-android)
else()
    include_directories(${ANDROID_SYSROOT}/usr/include/arm-linux-androideabi)
endif()

SET(CMAKE_BUILE_TYPE DEBUG)

SET(CMAKE_CXX_FLAGS_DEBUG "-g -Wall")

# 隐藏符号表，使IDA反编译函数导出隐藏
set (CMAKE_CXX_FLAGS "-fvisibility=hidden -fvisibility-inlines-hidden")

#设置生成的so动态库最后输出的路径
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/../jni_output/${ANDROID_ABI})

# 通配符寻找所有cpp
file(GLOB cppSrc "${PROJECT_SOURCE_DIR}/*.cpp")

MESSAGE(STATUS "cpp路径： ${cppSrc}")

# 引入log android库
find_library(log-lib log )
find_library(android-lib android )

# 主工程
add_library(PopupNdkHook SHARED ${cppSrc} )

#链接预编译和系统库
target_link_libraries(PopupNdkHook ${log-lib} ${android-lib})
