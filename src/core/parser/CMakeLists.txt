
add_library(parser STATIC ingredientsextractor.cpp recipeparser.cpp recipedocument.cpp)
target_link_libraries(parser PUBLIC Qt::Core Qt::Gui)
target_include_directories(parser INTERFACE .)
target_include_directories(parser PUBLIC ..)
