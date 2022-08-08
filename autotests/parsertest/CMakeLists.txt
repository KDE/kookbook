
add_executable(ingredientsextractortest ingredientsextractortest.cpp)
target_link_libraries(ingredientsextractortest PRIVATE Qt::Test parser)
add_test(ingredientsextractortest ingredientsextractortest) 

add_executable(recipeparsertest recipeparsertest.cpp)
target_link_libraries(recipeparsertest PRIVATE Qt::Test parser)
add_test(recipeparsertest recipeparsertest)
