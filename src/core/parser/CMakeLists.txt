
add_library(parser STATIC ingredientsextractor.cpp recipeparser.cpp recipedocument.cpp)
target_link_libraries(parser PUBLIC Qt5::Core Qt5::Gui)
target_include_directories(parser INTERFACE .)
target_include_directories(parser PUBLIC ..)
