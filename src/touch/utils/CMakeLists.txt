add_library(utils STATIC dirmodel.cpp standardpaths.cpp)
target_link_libraries(utils PUBLIC Qt::Core Qt::Qml)
