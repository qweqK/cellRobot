%language "c++"
%define api.value.type variant
%define api.namespace {yy}
%define parse.error verbose

%code requires {
    #include <string>
    extern int yylineno;
    void error(const std::string &msg);
}

%code{
    int yylex(yy::parser::value_type * yylval);
}

%token PLUS
%token <int> NUM
%type <int> expr
%left PLUS

%%

    prog:
        expr {std::cout << $1 << std::endl;}



    expr:
        NUM {$$ = $1;}
        |expr PLUS expr {$$ = $1 + $3;}
%%