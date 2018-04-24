
add_library(parser STATIC ingredientsextractor.cpp recipeparser.cpp)
target_link_libraries(parser PUBLIC Qt5::Core)
target_include_directories(parser INTERFACE .)
