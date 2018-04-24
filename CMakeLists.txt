project(kookbook)
cmake_minimum_required(VERSION 3.7)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(KF5Parts NO_MODULE)
find_package(KF5Service)
find_package(KF5Parts)
find_package(Qt5 COMPONENTS Widgets)

set(CMAKE_AUTOMOC on)
set(CMAKE_AUTORCC on)

enable_testing()
add_subdirectory(src)
add_subdirectory(autotests)
