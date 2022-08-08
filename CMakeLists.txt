project(kookbook)
cmake_minimum_required(VERSION 3.16)

set(QT_MIN_VERSION "5.15.2")
set(KF5_REQUIRED_VERSION "5.90.0")
set(KDE_COMPILERSETTINGS_LEVEL "5.82.0")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake/modules)
find_package(ECM REQUIRED)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${ECM_MODULE_PATH})
include(KDEInstallDirs)
include(KDECMakeSettings)
include(ECMInstallIcons)
include(ECMAddAppIcon)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Qt${QT_MAJOR_VERSION} COMPONENTS Widgets PrintSupport Qml)

set(CMAKE_AUTOMOC on)
set(CMAKE_AUTORCC on)

enable_testing()
add_subdirectory(src)
add_subdirectory(autotests)
add_subdirectory(converter)
