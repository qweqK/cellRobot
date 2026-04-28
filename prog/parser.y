
%language "c++"
%output "parser.cpp"
%defines "parser.hpp"
%define api.value.type variant
%define api.namespace {yy}


%parse-param {Data &data}

%code requires {
    #include "data.h"
    extern int yylineno;
    #include <string>
    #include <iostream>
    void error(const std::string &msg);


}

%code{
    int yylex(yy::parser::semantic_type * yylval);
     void yy::parser::error(const std::string &msg) {
        std::cerr << msg << yylineno << std::endl;
    }
    //  std::unique_ptr<std::unordered_map<std::string, SIT>> varstCur = std::make_unique<std::unordered_map<std::string, SIT>>();
}



%token CONST CALL TESTREP TESTONCE FUNC SIGNED UNSIGNED CELL MATRIX ASSIGN  BOTTOM XRAY
%token <std::string> VAR
%token <unsigned int> UNUM
%token <int> NUM
%token <int> TOP LEFT RIGHT  NTOP DOWN NDOWN NLEFT NRIGHT
%type <std::unique_ptr<VlueTypeNode>> expr
//%type <std::vector<int>> cellArg
%type<std::unique_ptr<Node>> sent init sentces
%type <int> noWall
%type <int> yesWall
%type <std::vector<std::pair<bool, bool>>> cellArg


%left '+' '-'
%left GE LE EQ NE '>' '<'
%left '*' '/' '%'
%nonassoc VAR
%nonassoc UMINUS
%left ')' '('
%right ASSIGN

%%

start:
    start function {
    if(data.functions.contains("start")) {
        data.functions["start"].root->print();
        data.functions["start"].root->proc();
    }
    std::cout << "f" << std::endl;
    }

    | {std::cout << "fq" << std::endl;}
    ;

function:
    FUNC VAR '{'sentces'}' {
    data.functions.emplace($2, Function(std::move(data.varstCur), std::move($4))) ;
    data.varstCur = std::make_unique<std::unordered_map<std::string, SIT>>();
    std::cout << "dat" << std::endl;
    }


 /*
params:
    | param
    | params ',' param
    ;
param:

*/


sent:
    ';' {$$ = std::make_unique<EmptyNode>();}
     | expr ';' {if(!$1) std::cout << "nullptr Expros" <<std::endl; $$ = std::move($1); }
     | init {$$ = std::move($1);}
     |TESTREP '(' expr ')' sent {$$ = make_unique<TestRepNode>(std::move($3), std::move($5));}
     | TESTONCE '(' expr ')' sent {$$ = make_unique<TestOnceNode>(std::move($3), std::move($5));}
     | '{' sentces '}' {$$ = std::move($2); }
     | TOP ';' {}
     | BOTTOM ';' {}
     | LEFT ';' {}
     | RIGHT ';' {}
     | CALL VAR '(' vars ')' ';' {}
     | VAR ASSIGN expr ';'{$$ = make_unique<AssignNode>($1, std::move($3), *data.varstCur);}
     | VAR '('UNUM ',' UNUM ')' ASSIGN expr {}
     ;




sentces:
     sent { if(!$1) std::cout << "nullptr sent" <<std::endl; $$ = std::move($1);}
     |sentces sent { if(!$1 || !$2) std::cout << "nullptr sents" <<std::endl ;$$ = make_unique<EndSentNode>(std::move($1), std::move($2));}
    ;


vars:
    VAR
    | vars VAR
    ;

expr:
    NUM {$$=std::make_unique<LiterNode>($1, VarType::SIGNED); }
    | UNUM {$$ =  std::make_unique<LiterNode>($1, VarType::UNSIGNED);}
    | XRAY {}
    | VAR '('UNUM ',' UNUM ')' {}
    | '-'expr %prec UMINUS {$$ = std::make_unique<UminusNode>(std::move($2));}
    | VAR {$$ = std::make_unique<VarNode>($1, *data.varstCur);}
    | expr '+' expr { $$ = std::make_unique<PlusNode>(std::move($1),std::move($3)); }
    | expr '-' expr { $$ = std::make_unique<MinusNode>(std::move($1),std::move($3)); }
    | expr '*' expr { $$ = std::make_unique<MulNode>(std::move($1),std::move($3));}
    | expr '/' expr { $$ = std::make_unique<DivNode>(std::move($1),std::move($3));}
    | expr '%' expr { $$ = std::make_unique<DivModNode>(std::move($1),std::move($3));}
    | '(' expr ')' { $$ = std::move($2);}
    | expr '<' expr { /*$$ = ($1 < $3);*/}
    | expr '>' expr {}
    | expr EQ expr {}
    ;

init:
    CONST UNSIGNED VAR ASSIGN expr ';'{ $$ = std::make_unique<InitNode>($3, std::move($5), *data.varstCur, VarType::UNSIGNED, true);}
    |CONST SIGNED VAR ASSIGN  expr ';' {$$ = std::make_unique<InitNode>($3, std::move($5), *data.varstCur,VarType::SIGNED, true);}
    |CONST CELL VAR ASSIGN '(' cellArg ')' ';' {$$ = std::make_unique<InitCellNode>($3, $6, *data.varstCur, true);}
    | MATRIX SIGNED VAR '(' UNUM ',' UNUM ')' ';'  {$$ = std::make_unique<InitMatrixNode>($3, $5, $7, VarType::SIGNED, *data.varstCur);}
    | MATRIX UNSIGNED VAR '(' UNUM ',' UNUM ')' ';' {$$ = std::make_unique<InitMatrixNode>($3, $5, $7, VarType::UNSIGNED, *data.varstCur);}
    | MATRIX CELL VAR  '(' UNUM ',' UNUM ')' ';' {$$ = std::make_unique<InitMatrixNode>($3, $5, $7, VarType::CELL, *data.varstCur);}

    |UNSIGNED VAR ASSIGN expr ';' {$$ = std::make_unique<InitNode>($2, std::move($4), *data.varstCur, VarType::UNSIGNED);}
    |SIGNED VAR ASSIGN expr ';'{ $$ = std::make_unique<InitNode>($2, std::move($4), *data.varstCur,VarType::SIGNED);}
    |CELL VAR ASSIGN '(' cellArg ')' ';'{ $$ = std::make_unique<InitCellNode>($2, $5,*data.varstCur);}
    |UNSIGNED VAR ';'{ }
    |SIGNED VAR';' { }
    |CELL VAR ';' {}


cellArg:
    noWall {
        std::vector<std::pair<bool, bool>> v(4, {false, false});
        v[$1] = {false, true};
        $$ = std::move(v);
    }


    |yesWall {
         std::vector<std::pair<bool, bool>> v(4, {false, false});
         v[$1] = {true, true};
         $$ = std::move(v);
    }


    | cellArg ',' noWall {
        if($1[$3].second) error("already est takou noWall");
        $1[$3] = {false, true};
        $$ = std::move($1);
    }


    | cellArg ',' yesWall {
      if($1[$3].second) error("already est takou noWall");
      $1[$3] = {true, true};
      $$ = std::move($1);
    }



noWall:
    NTOP { $$ = 0;}
    | NDOWN {$$ = 1;}
    | NLEFT {$$ = 2;}
    | NRIGHT {$$ = 3;}

yesWall:
    TOP { $$ = 0;}
    | DOWN {$$ = 1;}
    | LEFT {$$ = 2;}
    | RIGHT {$$ = 3;}

//asg:
   //VAR  ASSIGN expr ';' {}




%%
