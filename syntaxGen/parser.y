%{
    #include <stdio.h>
    #include "help.h"
    int yylex(void)

%}
%skeleton "lalr1.cc"
%require "3.2"

%defines
%define api.namespace  {MyParser}
//%define api.parser.class {Parser}

%define api.token.constructor
%union {
    int val;
    unsigned int uVal;
    std::string varInd;
    std::vector<std::vector<value>> matr;
    Cell cell;
}

%token CONST INT CALL TESTREP TESTONCE FUNC SIGNED UNSIGNED CELL MATRIX ASSIGN NAMEF TOP BOTTOM LEFT RIGHT XRAY


/*
%token <val> SIGNED
%token <uVal> UNSIGNED
%token <cell> CELL
%token <matr> MATRIX
*/


%left PLUS MINUS
%left GE LE EQ NE G L
%left MULTY DIVISION


%%




start:
    start stmt

    ;



porc:
    proc stmt {run($2);}
    |
    ;

/*
function:
    FUNC NAMEF '(' params ')' stmts


params:
    | param
    | params ',' param
    ;
param:
*/


stmt:
    ';'
     | init {}
     |TESTREP '(' expr ')' stmt {}
     | TESTONCE '(' expr ')' stmt {}
     | '{' stmts '}' {}
     | TOP ';' {}
     | BOTTOM ';' {}
     | LEFT ';' {}
     | RIGHT ';' {}
     ;

stmts:
    |stmt
     stmts stmt
    ;



expr:
    INT
    | XRAY
    | VAR
    | expr '+' expr { }
    | expr '-' expr { }
    | expr '*' expr { }
    | expr '/' expr { }
    | '(' expr ')' { }
    | expr '<' expr {}
    | expr '>' expr {}
    | expr GE expr {}
    | expr LE expr {}
    | expr EQ expr {}
    ;

init:
    CONST UNSIGNED VAR ASSIGN expr ';'{}
    |CONST SIGNED VAR ASSIGN  expr ';' { }
    |CONST CELL VAR  to ';' {}
    | MATRIX SIGNED VAR '(' INT ',' INT ')' ';' {}
    | MATRIX UNSIGNED VAR '(' INT ',' INT ')' ';' {}
    | MATRIX CELL VAR '(' INT ',' INT ')' ';' {}

    |UNSIGNED VAR ASSIGN expr ';' { }
    |SIGNED VAR ASSIGN expr ';'{ }
    |CELL VAR ASSIGN expr ';'{}
    |UNSIGNED VAR ';'{ }
    |SIGNED VAR';' { }
    |CELL VAR ';' {}

asg:
    VAR '='expr ';' {}
 %%




