add_executable(kookbooktouch
    main.cpp
    scanneradapter.cpp
    documentloader.cpp
    resources.qrc
    ${CMAKE_SOURCE_DIR}/doc/docs.qrc
    ${CMAKE_SOURCE_DIR}/icons/icons.qrc )
target_link_libraries(kookbooktouch PRIVATE Qt::Qml Qt::Gui scanner parser utils)
add_subdirectory(utils)

install(TARGETS kookbooktouch DESTINATION ${KDE_INSTALL_BINDIR})
install(FILES org.kde.kookbooktouch.desktop DESTINATION ${KDE_INSTALL_APPDIR})
