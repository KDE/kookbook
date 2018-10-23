add_executable(kookbooktouch
    main.cpp
    scanneradapter.cpp
    documentloader.cpp
    resources.qrc
    ${CMAKE_SOURCE_DIR}/doc/docs.qrc
    ${CMAKE_SOURCE_DIR}/icons/icons.qrc )
target_link_libraries(kookbooktouch PRIVATE Qt5::Qml Qt5::Gui scanner parser utils)
add_subdirectory(utils)

