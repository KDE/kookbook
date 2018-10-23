add_library(utils STATIC dirmodel.cpp standardpaths.cpp)
target_link_libraries(utils PUBLIC Qt5::Core Qt5::Qml)
