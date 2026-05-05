add_library(visual STATIC visualiser.h visualiser.cpp)
target_link_libraries( visual PUBLIC
        sfml-graphics
        sfml-window
        sfml-system
)

target_include_directories(visual PUBLIC    ${CMAKE_CURRENT_BINARY_DIR}/../prog
        ${CMAKE_CURRENT_SOURCE_DIR}../prog)