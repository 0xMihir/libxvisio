cmake_minimum_required(VERSION 3.26)
project(xvisio)

set(CMAKE_CXX_STANDARD 23)
add_library(xvisio SHARED src/device/device.cpp
        src/xvisio.cpp
        src/device/hid.cpp
        src/slam.cpp
        src/types/pose.cpp
)

target_compile_options(xvisio PRIVATE -Wno-deprecated-enum-enum-conversion)
target_include_directories(xvisio PRIVATE include/libxvisio/ include/libxvisio/device include/libxvisio/types)

add_executable(example example/main.cpp example/main.h)
target_compile_options(example PRIVATE -Wno-deprecated-enum-enum-conversion)
# libusb
find_package(PkgConfig REQUIRED)
pkg_check_modules(libUSB REQUIRED libusb-1.0)

target_link_libraries(xvisio ${libUSB_LINK_LIBRARIES})
target_include_directories(xvisio PUBLIC ${libUSB_INCLUDE_DIRS})
target_compile_options(xvisio PUBLIC ${libUSB_CFLAGS_OTHER})

target_link_libraries(example ${libUSB_LINK_LIBRARIES})
target_link_libraries(example xvisio)
target_include_directories(example PUBLIC ${libUSB_INCLUDE_DIRS})
target_include_directories(example PRIVATE include/libxvisio/ include/libxvisio/device include/libxvisio/types)
target_compile_options(example PUBLIC ${libUSB_CFLAGS_OTHER})
