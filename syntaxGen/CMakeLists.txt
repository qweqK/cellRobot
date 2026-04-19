find_package(BISON REQUIRED)
BISON_TARGET(mParser parser.y lexer.tab.cpp)
