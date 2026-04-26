find_package(FLEX REQUIRED)

FLEX_TARGET(mLexer ../prog/lexer.l ${CMAKE_CURRENT_BINARY_DIR}/lexer.cpp )
