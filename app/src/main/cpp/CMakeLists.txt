# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

project("Fa4Lib")

cmake_minimum_required(VERSION 3.10.2)

set(CMAKE_CXX_STANDART 14)
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()
message(STATUS "!!!! CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")
set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O2")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -pthread")
message(STATUS "!!!! CMAKE_CXX_FLAGS_RELEASE: ${CMAKE_CXX_FLAGS_RELEASE}")

# Declares and names the project.

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.

file(GLOB CPP
        Source/*.cpp
        )
file(GLOB H
        Include/*.hpp
        )

add_library( # Sets the name of the library.
            Fa4-native-lib

             # Sets the library as a shared library.
             SHARED

             # Provides a relative path to your source file(s).

        Include/FindA4.hpp
        Include/Vector.hpp
        Include/Tabu.hpp

        Source/Vector.cpp
        Source/FindA4.cpp
        Source/GetA4.cpp
        Source/GetCamPos.cpp
        Source/GetKrit.cpp
        Source/Init.cpp
        Source/UpdateOreint.cpp
        Source/Tabu.cpp
        Source/MakeBest.cpp

        Nativelib.cpp


        )

add_library( # Sets the name of the library.
        Fa4-native-lib-test

        # Sets the library as a shared library.
        SHARED

        # Provides a relative path to your source file(s).

        Include/FindA4.hpp
        Include/Vector.hpp
        Include/Tabu.hpp
        Include/Autotest.hpp

        Source/Vector.cpp
        Source/FindA4.cpp
        Source/GetA4.cpp
        Source/GetCamPos.cpp
        Source/GetKrit.cpp
        Source/Init.cpp
        Source/UpdateOreint.cpp
        Source/Tabu.cpp
        Source/MakeBest.cpp
        Source/AutoTest.cpp

        Nativelib-test.cpp

        )

# Searches for a specified prebuilt library and stores the path as a
# variable. Because CMake includes system libraries in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.

#find_library( # Sets the name of the path variable.
#              log-lib

              # Specifies the name of the NDK library that
              # you want CMake to locate.
#              log )

# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.

#target_link_libraries( # Specifies the target library.
#                       native-lib

                       # Links the target library to the log library
                       # included in the NDK.
#                       ${log-lib} )