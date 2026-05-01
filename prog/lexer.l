%{
#include "parser.hpp"
#define YY_DECL int yylex(yy::parser::semantic_type * yylval)
%}

%option noyywrap
%option yylineno

%%

"const" {return yy::parser::token::CONST;}
"call" {return yy::parser::token::CALL;}
"testrep" {return yy::parser::token::TESTREP;}
"testonce" {return yy::parser::token::TESTONCE;}
"func" {return yy::parser::token::FUNC;}
"signed" {return yy::parser::token::SIGNED;}
"unsigned" {return yy::parser::token::UNSIGNED;}
"<-" {return yy::parser::token::ASSIGN;}
">=" {return yy::parser::token::GE;}
"<=" {return yy::parser::token::LE;}
"==" {return yy::parser::token::EQ;}
"!=" {return yy::parser::token::NE;}
"cell" {return yy::parser::token::CELL;}
"matrix" {return yy::parser::token::MATRIX;}
"top" { yylval->emplace<int>(0); return yy::parser::token::TOP;}
"down" {yylval->emplace<int>(1); return yy::parser::token::DOWN;}
"left" {yylval->emplace<int>(2); return yy::parser::token::LEFT;}
"right" {yylval->emplace<int>(3); return yy::parser::token::RIGHT;}
"ntop" {yylval->emplace<int>(0); return yy::parser::token::NTOP;}
"ndown" {yylval->emplace<int>(1); return yy::parser::token::NDOWN;}
"nright" {yylval->emplace<int>(3); return yy::parser::token::NRIGHT;}
"nleft" {yylval->emplace<int>(2); return yy::parser::token::NLEFT;}
"printvar" {yylval->emplace<int>(2); return yy::parser::token::PRINTVAR;}
"xray" {return yy::parser::token::XRAY;}
[0-9]+ {yylval->emplace<int>(std::stoi(yytext)); return yy::parser::token::NUM;}
[0-9]+u {std::string s(yytext, yyleng-1); yylval->emplace<unsigned int>(std::stoul(s)); return yy::parser::token::UNUM;}
[a-zA-Z][a-zA-Z0-9]* { yylval->emplace<std::string>(yytext); return yy::parser::token::VAR;}
"//"[^\n]* {}
[-{}()><=+*/;{},%] { return *yytext;}
[ \n\t\r] {}

<<EOF>> {return 0;}

. { std::cout << "bred" << yytext << "\n"; }


%%





