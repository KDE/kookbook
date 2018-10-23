project(kookbook)
cmake_minimum_required(VERSION 3.7)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake/modules)
find_package(ECM REQUIRED)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${ECM_MODULE_PATH})
include(KDEInstallDirs)
include(ECMInstallIcons)
include(ECMAddAppIcon)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Qt5 COMPONENTS Widgets PrintSupport Qml)
find_package(Discount REQUIRED)

set(CMAKE_AUTOMOC on)
set(CMAKE_AUTORCC on)

enable_testing()
add_subdirectory(src)
add_subdirectory(autotests)
