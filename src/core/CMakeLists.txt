find_package(Threads)

add_library(activedocument STATIC activedocument.cpp)
target_include_directories(activedocument PUBLIC .)
target_link_libraries(activedocument PUBLIC Qt::Core)

add_library(scanner STATIC scanner.cpp)
target_include_directories(scanner PUBLIC .)
target_link_libraries(scanner PUBLIC Qt::Core parser PRIVATE ${CMAKE_THREAD_LIBS_INIT})

add_subdirectory(parser)
