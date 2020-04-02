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
add_library( avcodec-58
             SHARED
             IMPORTED )
set_target_properties( avcodec-58
                       PROPERTIES IMPORTED_LOCATION
        ${CMAKE_CURRENT_SOURCE_DIR}/libs/armeabi-v7a/libavcodec.so )
add_library( avfilter-7
             SHARED
             IMPORTED )
set_target_properties( avfilter-7
                       PROPERTIES IMPORTED_LOCATION
        ${CMAKE_CURRENT_SOURCE_DIR}/libs/armeabi-v7a/libavfilter.so )
add_library( avformat-58
             SHARED
             IMPORTED )
set_target_properties( avformat-58
                       PROPERTIES IMPORTED_LOCATION
        ${CMAKE_CURRENT_SOURCE_DIR}/libs/armeabi-v7a/libavformat.so )
add_library( avutil-56
             SHARED
             IMPORTED )
set_target_properties( avutil-56
                       PROPERTIES IMPORTED_LOCATION
        ${CMAKE_CURRENT_SOURCE_DIR}/libs/armeabi-v7a/libavutil.so )
add_library( swresample-3
             SHARED
             IMPORTED )
set_target_properties( swresample-3
                       PROPERTIES IMPORTED_LOCATION
        ${CMAKE_CURRENT_SOURCE_DIR}/libs/armeabi-v7a/libswresample.so )
add_library( swscale-5
             SHARED
             IMPORTED )
set_target_properties( swscale-5
                       PROPERTIES IMPORTED_LOCATION
        ${CMAKE_CURRENT_SOURCE_DIR}/libs/armeabi-v7a/libswscale.so )
add_library( postproc-55
        SHARED
        IMPORTED )
set_target_properties( postproc-55
        PROPERTIES IMPORTED_LOCATION
        ${CMAKE_CURRENT_SOURCE_DIR}/libs/armeabi-v7a/libpostproc.so )

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
                       avcodec-58
                       avfilter-7
                       avformat-58
                       avutil-56
                       swresample-3
                       swscale-5
                       postproc-55
                       # Links the target library to the log library
                       # included in the NDK.
                       ${log-lib}  ${android-lib})
######