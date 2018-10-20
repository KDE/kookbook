
add_library(panes metadatapane.cpp fspane.cpp mainpane.cpp rawviewpane.cpp panebase.cpp treepane.cpp listpane.cpp ingredientsparserpane.cpp)
target_link_libraries(panes PUBLIC Qt5::Widgets Qt5::PrintSupport PRIVATE parser activedocument)
target_include_directories(panes INTERFACE .)
target_include_directories(panes PUBLIC ..)
