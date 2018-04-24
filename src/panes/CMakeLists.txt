
add_library(panes metadatapane.cpp fspane.cpp mainpane.cpp rawviewpane.cpp panebase.cpp treepane.cpp listpane.cpp)
target_link_libraries(panes PUBLIC Qt5::Widgets KF5::Service KF5::Parts PRIVATE parser)
target_include_directories(panes INTERFACE .)
target_include_directories(panes PUBLIC ..)
