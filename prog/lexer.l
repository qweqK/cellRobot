%{
#include "parser.hpp"
#define YY_DECL int yylex(yy::parser::semantic_type * yylval, yy::parser::location_type * yylloc )
#define YYLTYPE yy::parser::location_type
%}

%option noyywrap
%option yylineno

%%
%{
    yylloc -> step();
%}
"const" {yylloc->columns(yyleng); return yy::parser::token::CONST;}
"call" {yylloc->columns(yyleng); return yy::parser::token::CALL;}
"testrep" {yylloc->columns(yyleng); return yy::parser::token::TESTREP;}
"testonce" {yylloc->columns(yyleng); return yy::parser::token::TESTONCE;}
"func" {yylloc->columns(yyleng); return yy::parser::token::FUNC;}
"signed" {yylloc->columns(yyleng); return yy::parser::token::SIGNED;}
"unsigned" {yylloc->columns(yyleng); return yy::parser::token::UNSIGNED;}
"<-" {yylloc->columns(yyleng); return yy::parser::token::ASSIGN;}
">=" {yylloc->columns(yyleng); return yy::parser::token::GE;}
"<=" {yylloc->columns(yyleng); return yy::parser::token::LE;}
"==" {yylloc->columns(yyleng); return yy::parser::token::EQ;}
"!=" {yylloc->columns(yyleng); return yy::parser::token::NE;}
"cell" {yylloc->columns(yyleng); return yy::parser::token::CELL;}
"matrix" {yylloc->columns(yyleng); return yy::parser::token::MATRIX;}
"top" {yylloc->columns(yyleng);  yylval->emplace<int>(0); return yy::parser::token::TOP;}
"down" {yylloc->columns(yyleng); yylval->emplace<int>(1); return yy::parser::token::DOWN;}
"left" {yylloc->columns(yyleng); yylval->emplace<int>(2); return yy::parser::token::LEFT;}
"right" {yylloc->columns(yyleng); yylval->emplace<int>(3); return yy::parser::token::RIGHT;}
"ntop" {yylloc->columns(yyleng); yylval->emplace<int>(0); return yy::parser::token::NTOP;}
"iswin" {yylloc->columns(yyleng); yylval->emplace<int>(0); return yy::parser::token::ISWIN;}
"ndown" {yylloc->columns(yyleng); yylval->emplace<int>(1); return yy::parser::token::NDOWN;}
"bottom" {yylloc->columns(yyleng); yylval->emplace<int>(1); return yy::parser::token::BOTTOM;}
"nright" {yylloc->columns(yyleng); yylval->emplace<int>(3); return yy::parser::token::NRIGHT;}
"nleft" {yylloc->columns(yyleng); yylval->emplace<int>(2); return yy::parser::token::NLEFT;}
"printvar" {yylloc->columns(yyleng); yylval->emplace<int>(2); return yy::parser::token::PRINTVAR;}
"printmap" {yylloc->columns(yyleng); yylval->emplace<int>(2); return yy::parser::token::PRINTMAP;}
"printscr" {yylloc->columns(yyleng); return yy::parser::token::PRINSCREEN; }
"xray" {yylloc->columns(yyleng); return yy::parser::token::XRAY;}
[0-9]+ {yylloc->columns(yyleng); yylval->emplace<int>(std::stoi(yytext)); return yy::parser::token::NUM;}
[0-9]+u {yylloc->columns(yyleng); std::string s(yytext, yyleng-1); yylval->emplace<unsigned int>(std::stoul(s)); return yy::parser::token::UNUM;}
[a-zA-Z][a-zA-Z0-9]* {yylloc->columns(yyleng);  yylval->emplace<std::string>(yytext); return yy::parser::token::VAR;}
"//"[^\n]* {yylloc->columns(yyleng); yylloc->step();}
[-{}()><=+*/;{},%#] { yylloc->columns(yyleng); return *yytext;}
\r?\n {
    yylloc->lines(1);
    yylloc->step();
}

[ \t] {
    yylloc->columns(yyleng);
    yylloc->step();
}

<<EOF>> {return 0;}

. {std::cerr << std::string(yytext) << std::endl; }

%%





