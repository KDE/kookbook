

set(KOOKBOOK_ICONS_PNG
    ${CMAKE_SOURCE_DIR}/icons/16-apps-kookbook.png
    ${CMAKE_SOURCE_DIR}/icons/22-apps-kookbook.png
    ${CMAKE_SOURCE_DIR}/icons/32-apps-kookbook.png
    ${CMAKE_SOURCE_DIR}/icons/48-apps-kookbook.png
    ${CMAKE_SOURCE_DIR}/icons/64-apps-kookbook.png
    ${CMAKE_SOURCE_DIR}/icons/128-apps-kookbook.png
)

set(KOOKBOOK_ICONS_SVG
${CMAKE_SOURCE_DIR}/icons/sc-apps-kookbook.svgz
)

set(KOOKBOOK_SRCS main.cpp mainwindow.cpp ${CMAKE_SOURCE_DIR}/doc/docs.qrc ${CMAKE_SOURCE_DIR}/icons/icons.qrc)

ecm_add_app_icon(KOOKBOOK_SRCS ICONS ${KOOKBOOK_ICONS_PNG})

add_subdirectory(panes)

set(CMAKE_WIN32_EXECUTABLE on)
add_executable(kookbook ${KOOKBOOK_SRCS})

target_link_libraries(kookbook PRIVATE panes parser scanner activedocument)

install(TARGETS kookbook DESTINATION ${KDE_INSTALL_BINDIR})
install(FILES org.kde.kookbook.desktop DESTINATION ${KDE_INSTALL_APPDIR})
install(FILES org.kde.kookbook.appdata.xml DESTINATION ${KDE_INSTALL_METAINFODIR})
ecm_install_icons(ICONS ${KOOKBOOK_ICONS_PNG} ${KOOKBOOK_ICONS_SVG} DESTINATION ${ICON_INSTALL_DIR} THEME hicolor)
