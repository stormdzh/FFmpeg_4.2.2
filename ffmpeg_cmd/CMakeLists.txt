cmake_minimum_required(VERSION 3.4.1)
# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for y
# Gradle automatically packages shared libraries with your APK.
file(GLOB native_srcs2 "src/main/cpp/*.c")
include_directories(src/main/cpp)
include_directories(src/main/h )
get_directory_property(MY_DIRECTORY_PARENT PARENT_DIRECTORY)
message("CMAKE_SOURCE_DIR = ${CMAKE_SOURCE_DIR}")
message("CMAKE_CURRENT_SOURCE_DIR = ${CMAKE_CURRENT_SOURCE_DIR}")
message("PROJECT_SOURCE_DIR = ${MY_DIRECTORY_PARENT}")

include_directories(../${MY_DIRECTORY_PARENT}/ffmpeg-4.2.2)

add_library( # Sets the name of the library.
             ffmpeginvoke
             # Sets the library as a shared library.
             SHARED
        ${native_srcs2} )



add_library( dzhffmpeg
        SHARED
        IMPORTED )
set_target_properties( dzhffmpeg
        PROPERTIES IMPORTED_LOCATION
        ${CMAKE_CURRENT_SOURCE_DIR}/libs/armeabi-v7a/libdzhffmpeg.so )



include_directories( libs/include )
find_library( # Sets the name of the path variable.
              log-lib
              # Specifies the name of the NDK library that
              # you want CMake to locate.
              log )
find_library( # Sets the name of the path variable.
              android-lib
              # Specifies the name of the NDK library that
              # you want CMake to locate.
              android)




# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.
target_link_libraries( # Specifies the target library.
                       ffmpeginvoke
                       dzhffmpeg
                       # Links the target library to the log library
                       # included in the NDK.
                       ${log-lib}  ${android-lib})
######