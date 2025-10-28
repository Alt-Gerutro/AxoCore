add_library(timer STATIC
    pit.c
)

target_include_directories(timer PUBLIC ${CMAKE_SOURCE_DIR}/include)