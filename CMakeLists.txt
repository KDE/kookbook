cmake_minimum_required(VERSION 3.16)

project(kookbook)

set(QT_MIN_VERSION "5.15.2")
set(KF_MIN_VERSION "5.92.0")
set(KDE_COMPILERSETTINGS_LEVEL "5.82.0")

find_package(ECM ${KF_MIN_VERSION})
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH})

include(KDEInstallDirs)
include(KDECompilerSettings NO_POLICY_SCOPE)
include(KDECMakeSettings)
include(ECMInstallIcons)
include(ECMAddAppIcon)
include(ECMAddTests)
include(ECMDeprecationSettings)
if (QT_MAJOR_VERSION STREQUAL "6")
    set(QT_REQUIRED_VERSION "6.4.0")
    set(KF_MIN_VERSION "5.240.0")
    set(KF_MAJOR_VERSION "6")
else()
    set(KF_MAJOR_VERSION "5")
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Qt${QT_MAJOR_VERSION} ${QT_MIN_VER} CONFIG REQUIRED COMPONENTS Widgets PrintSupport Qml)

ecm_set_disabled_deprecation_versions(QT 6.4
    KF 5.102
)

add_subdirectory(src)
add_subdirectory(autotests)
add_subdirectory(converter)
