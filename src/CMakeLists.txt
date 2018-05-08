
find_package(Threads)
add_subdirectory(parser)
add_subdirectory(panes)
add_executable(kookbook main.cpp mainwindow.cpp activedocument.cpp scanner.cpp doc/docs.qrc)

target_link_libraries(kookbook PRIVATE panes parser ${CMAKE_THREAD_LIBS_INIT})

install(TARGETS kookbook DESTINATION ${KDE_INSTALL_BINDIR})
install(FILES kookbook.desktop DESTINATION ${KDE_INSTALL_APPDIR})
