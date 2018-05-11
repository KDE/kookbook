
find_package(Threads)

set(KOOKBOOK_ICONS_PNG
    ${CMAKE_CURRENT_SOURCE_DIR}/icons/16-apps-kookbook.png
    ${CMAKE_CURRENT_SOURCE_DIR}/icons/22-apps-kookbook.png
    ${CMAKE_CURRENT_SOURCE_DIR}/icons/32-apps-kookbook.png
    ${CMAKE_CURRENT_SOURCE_DIR}/icons/48-apps-kookbook.png
    ${CMAKE_CURRENT_SOURCE_DIR}/icons/64-apps-kookbook.png
    ${CMAKE_CURRENT_SOURCE_DIR}/icons/128-apps-kookbook.png
)

set(KOOKBOOK_ICONS_SVG
${CMAKE_CURRENT_SOURCE_DIR}/icons/sc-apps-kookbook.svgz
)

set(KOOKBOOK_SRCS main.cpp mainwindow.cpp activedocument.cpp scanner.cpp doc/docs.qrc icons/icons.qrc)

ecm_add_app_icon(KOOKBOOK_SRCS ICONS ${KOOKBOOK_ICONS_PNG})

add_subdirectory(parser)
add_subdirectory(panes)
add_executable(kookbook ${KOOKBOOK_SRCS})

target_link_libraries(kookbook PRIVATE panes parser ${CMAKE_THREAD_LIBS_INIT})

install(TARGETS kookbook DESTINATION ${KDE_INSTALL_BINDIR})
install(FILES kookbook.desktop DESTINATION ${KDE_INSTALL_APPDIR})
ecm_install_icons(ICONS ${KOOKBOOK_ICONS_PNG} ${KOOKBOOK_ICONS_SVG} DESTINATION ${ICON_INSTALL_DIR} THEME hicolor)
