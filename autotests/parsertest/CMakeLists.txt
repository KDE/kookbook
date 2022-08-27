
ecm_add_test(ingredientsextractortest.cpp
    LINK_LIBRARIES
        Qt::Test
        parser
)

ecm_add_test(recipeparsertest.cpp
    LINK_LIBRARIES
        Qt::Test
        parser
)
