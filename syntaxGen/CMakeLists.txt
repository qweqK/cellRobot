find_package(BISON REQUIRED)



BISON_TARGET(mParser ../prog/parser.y {}/parser.cpp DEFINES_FILE ./parser.hpp)

