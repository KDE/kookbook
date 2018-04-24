
find_package(Threads)
add_subdirectory(parser)
add_subdirectory(panes)
add_executable(kookbook main.cpp mainwindow.cpp activedocument.cpp scanner.cpp doc/docs.qrc)

target_link_libraries(kookbook PRIVATE panes parser ${CMAKE_THREAD_LIBS_INIT})
