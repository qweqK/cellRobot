%{
#include "parser2.tab.hh"
#define YY_DECL int yylex(yy::parser::value_type * yylval)
%}
%option batch
%option noyywrap
%option yylineno


%%
[0-9]+ {
       yylval->emplace<int>(std::stoi(yytext));
       return yy::parser::token::NUM;
    }

"+" {return yy::parser::token::PLUS;}

%%

