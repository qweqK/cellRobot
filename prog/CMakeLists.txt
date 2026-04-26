
include_directories(../lexGen)
include_directories(../syntaxGen)



find_package(BISON REQUIRED)
find_package(FLEX REQUIRED)

BISON_TARGET(mParser parser.y ./parser.cpp DEFINES_FILE ./parser.hpp)
FLEX_TARGET(mLexer lexer.l ./lexer.cpp )

#add_custom_command(OUTPUT ${FLEX_mLexer_OUTPUTS} DEPENDS ${BISON_mParser_OUTPUTS})

include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
add_executable(lab main.cpp ${BISON_mParser_OUTPUTS} ${FLEX_mLexer_OUTPUTS} game.cpp game.h)

#add_executable(lab main.cpp parser.cpp parser.hpp lex.yy.c)

#add_executable(lab2 main.cpp  parser2.tab.cc parser2.tab.hh lex.yy.cc)