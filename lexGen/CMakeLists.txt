find_package(FLEX REQUIRED)

FLEX_TARGET(mLexer lex.l lexer.yy.c)
