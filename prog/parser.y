
%language "c++"
%output "parser.cpp"
%defines "parser.hpp"
%define api.value.type variant
%define api.namespace {yy}
%locations
%define parse.error verbose


%parse-param {Data &data}


%code requires {
    #include "data.h"
    extern int yylineno;
    #include <string>
    #include <iostream>
    #include "arithNodes.h"
    #include "assignNodes.h"
    #include "baseNodes.h"
    #include "castPrekols.h"
    #include "differentNodes.h"
    #include "diffValues.h"
    #include "init.h"
    #include "funtionNodes.h"
    #include "robot.h"
    #include "robotActionNodes.h"
    void error(const std::string &msg);


}

%code{
    int yylex(yy::parser::semantic_type * yylval, yy::parser::location_type * yylloc);
     void yy::parser::error(const location_type & lt, const std::string &msg) {
        data.errorStatic(lt, msg);
    }
    //  std::unique_ptr<std::unordered_map<std::string, SIT>> varstCur = std::make_unique<std::unordered_map<std::string, SIT>>();
}



%token CONST CALL TESTREP TESTONCE FUNC SIGNED UNSIGNED CELL MATRIX ASSIGN  XRAY PRINTVAR PRINTMAP ISWIN PRINSCREEN
%token <std::string> VAR
%token <unsigned int> UNUM
%token <int> NUM
%token <int> TOP LEFT RIGHT  NTOP DOWN NDOWN NLEFT NRIGHT BOTTOM
%type <std::unique_ptr<VlueTypeNode>> expr
%type<std::unique_ptr<Node>> sent sentces
%type <int> noWall
%type <int> yesWall
%type <std::vector<std::pair<bool, bool>>> cellArg
%type <PARAMS> params
%type <std::unique_ptr<DefInitNode>>  init
%type <std::pair<std::string, SIT>> param
%type <std::pair<std::string, PARAMS>> preFunc
%type <std::vector<std::string>> varsc
%left '+' '-'
%left GE LE EQ NE '>' '<'
%left '*' '/' '%'
%nonassoc '#'
%nonassoc VAR
%nonassoc UMINUS
%left ')' '('
%right ASSIGN

%%

start:
    prog YYEOF {if(data.errorCounter ==0){

    data.interpritation();
    }
    else if(data.errorCounter ==1) {std::cout << data.errorCounter << " " << "error" << std::endl;}
    else { std::cerr << data.errorCounter << " " << "errors" << std::endl;}
    }
    ;
prog:
    prog function {}
    |prog init {data.varst->insert(std::make_pair($2->getName(), $2->getRightValue()));}
    | prog error ';' {yyerrok;}
    | prog error '}' {yyerrok;}
    |
    ;

figure_bracket_open:
    '{' {data.buildStorage.emplace_back();}

function:
    preFunc sentces '}' {

    //$2->print();
    data.fStore.emplace($1.first, Function(std::move($2), std::move($1.second)));
    while(data.buildStorage.size() > 1) data.buildStorage.pop_back();
    }
    ;

preFunc:
     FUNC VAR '('params')' '{' {
        $$ = std::make_pair(std::move($2), std::move($4));
        data.buildStorage.emplace_back();
        data.addBuildParams($$.second);
     }

params:
     param {$$.push_back($1);}
    | params ',' param {$$ = std::move($1); $$.push_back($3);}
    | {}
    ;
param:
    SIGNED VAR {$$  = std::make_pair(std::move($2), SIT(0, false));}
    |UNSIGNED VAR {$$  = std::make_pair(std::move($2), SIT(0u, false));}
    |MATRIX CELL VAR {$$  = std::make_pair(std::move($3), SIT(Matrix(VarType::SIGNED, 0, 0), false));}
    |MATRIX SIGNED VAR {$$  = std::make_pair(std::move($3), SIT(Matrix(VarType::UNSIGNED, 0, 0), false));}
    |MATRIX UNSIGNED VAR {$$  = std::make_pair(std::move($3), SIT(Matrix(VarType::CELL, 0, 0), false));}
    |CELL VAR {$$  = std::make_pair(std::move($2), SIT(Cell(), true));}
    |CONST SIGNED VAR {$$  = std::make_pair(std::move($3), SIT(0, true));}
    |CONST UNSIGNED VAR {$$  = std::make_pair(std::move($3), SIT(0u, true));}
    |CONST CELL VAR {$$  = std::make_pair(std::move($3), SIT(Cell(), true));}
    ;



sent:
    ';'                                      {$$ = std::make_unique<EmptyNode>();}
     | expr ';'                             { @$ = @1 + @2; $$ = std::move($1);}
     | init                                 {$$ = std::move($1);}
     |TESTREP '(' expr ')' sent             {@$ = @1 + @5; $$ = make_unique<TestRepNode>(std::move($3), std::move($5), data, @$);}
     | TESTONCE '(' expr ')' sent           {@$ = @1 + @5;$$ = make_unique<TestOnceNode>(std::move($3), std::move($5), data, @$);}
     | figure_bracket_open sentces '}'      {@$ = @1 + @3; $$ = std::make_unique<BracketNode>(std::move($2), data); data.buildStorage.pop_back();}
     | TOP ';'                              {@$ = @1 + @2; $$ = std::make_unique<MoveNode>(DIRECT::UP, data.map);}
     | BOTTOM ';'                           {@$ = @1 + @2; $$ = std::make_unique<MoveNode>(DIRECT::DOWN, data.map);}
     | LEFT ';'                             {@$ = @1 + @2; $$ = std::make_unique<MoveNode>(DIRECT::LEFT, data.map);}
     | RIGHT ';'                            {@$ = @1 + @2; $$ = std::make_unique<MoveNode>(DIRECT::RIGHT, data.map);}
     | VAR ASSIGN expr ';'                  {@$ = @1 + @4; $$ = make_unique<AssignNode>($1, std::move($3), data, @$);}
     | VAR '('expr ',' expr ')' ASSIGN expr ';'  {@$ = @1 + @8; $$ = std::make_unique<MatrixAccesNode>($1, std::move($8),data ,std::move($3), std::move($5), @$);}
     | PRINTVAR VAR ';' { $$ = std::make_unique<PrintNode>($2, data);}
     | error ';' {yyerrok;}
     | PRINTMAP ';' {$$ = std::make_unique<PrintMapNode>(data.map);}
     | ISWIN ';' {$$ = std::make_unique<IsWinNode>(data);}
     | PRINSCREEN ';' {$$ = std::make_unique<ScreenNode>(data);}
     ;




sentces:
     sent { $$ = std::move($1); }
     |sentces sent { @$ = @1 + @2; $$ = make_unique<EndSentNode>(std::move($1), std::move($2));}
     ;


expr:
    NUM {$$=std::make_unique<LiterNode>($1, VarType::SIGNED); }
    | UNUM {$$ =  std::make_unique<LiterNode>($1, VarType::UNSIGNED);}
    | XRAY {$$ = std::make_unique<XrayNode>(data.map);}
    | VAR '('expr ',' expr ')' {@$ = @1 + @6; $$ = std::make_unique<MatrixAssignAccesNode>($1, data, std::move($3), std::move($5), @$);}
    | '-'expr %prec UMINUS {@$ = @1 + @2; $$ = std::make_unique<UminusNode>(std::move($2), data, @$);}
    | VAR {$$ = std::make_unique<VarNode> ($1, data, @$);}
    | expr '+' expr {@$ = @1 + @3; $$ = std::make_unique<PlusNode>(std::move($1),std::move($3), data, @$); }
    | expr '-' expr {@$ = @1 + @3; $$ = std::make_unique<MinusNode>(std::move($1),std::move($3), data, @$); }
    | expr '*' expr {@$ = @1 + @3; $$ = std::make_unique<MulNode>(std::move($1),std::move($3), data, @$);}
    | expr '/' expr {@$ = @1 + @3; $$ = std::make_unique<DivNode>(std::move($1),std::move($3), data, @$);}
    | expr '%' expr {@$ = @1 + @3; $$ = std::make_unique<DivModNode>(std::move($1),std::move($3), data, @$);}
    | '(' expr ')' {@$ = @1 + @3; $$ = std::move($2);}
    | expr '>' expr { @$ = @1 + @3; $$ = std::make_unique<GreaterNode>(std::move($1), std::move($3), data, @$);}
    | expr '<' expr {@$ = @1 + @3; $$ = std::make_unique<LessNode>(std::move($1), std::move($3), data, @$);}
    | expr '=' expr {@$ = @1 + @3; $$ = std::make_unique<EqNode>(std::move($1), std::move($3), data, @$);}
    | '#' expr {@$ = @1 + @2; $$ = std::make_unique<MatrUnaryOper>(std::move($2), data, @$);}
    | CALL VAR '(' varsc ')' {@$ = @1 + @5; $$ = std::make_unique<CallFuncNode>($2, std::move($4), data, @$);}
    | CALL VAR '(' ')' {@$ = @1 + @4; $$ = std::make_unique<CallFuncNode>($2, data, @$);}
    ;


    varsc:
        VAR {$$.push_back($1);}
        | varsc  VAR {@$ = @1 + @2; $$ = std::move($1); $$.push_back($2);}


init:
    CONST UNSIGNED VAR ASSIGN expr ';'{ @$ = @1 + @6; $$ = std::make_unique<InitNode>($3, std::move($5), data, VarType::UNSIGNED, @$ ,true);}
    |CONST SIGNED VAR ASSIGN  expr ';' {@$ = @1 + @6;$$ = std::make_unique<InitNode>($3, std::move($5), data,VarType::SIGNED, @$,true);}
    |CONST CELL VAR ASSIGN '(' cellArg ')' ';' { @$ = @1 + @8;$$ = std::make_unique<InitCellNode>($3, $6, data, @$,true);}
    | MATRIX SIGNED VAR '(' expr ',' expr ')' ';'  { @$ = @1 + @9; $$ = std::make_unique<InitMatrixNode>($3, std::move($5), std::move($7), VarType::SIGNED, data, @$);}
    | MATRIX UNSIGNED VAR '(' expr ',' expr ')' ';' {@$ = @1 + @9; $$ = std::make_unique<InitMatrixNode>($3, std::move($5), std::move($7), VarType::UNSIGNED,data, @$);}
    | MATRIX CELL VAR  '(' expr ',' expr ')' ';' {@$ = @1 + @9; $$ = std::make_unique<InitMatrixNode>($3, std::move($5), std::move($7), VarType::CELL, data,@$);}

    |UNSIGNED VAR ASSIGN expr ';' {@$ = @1 + @5; $$ = std::make_unique<InitNode>($2, std::move($4), data, VarType::UNSIGNED, @$);}
    |SIGNED VAR ASSIGN expr ';'{ @$ = @1 + @5; $$ = std::make_unique<InitNode>($2, std::move($4), data,VarType::SIGNED, @$);}
    |CELL VAR ASSIGN '(' cellArg ')' ';'{ @$ = @1 + @7; $$ = std::make_unique<InitCellNode>($2, $5, data, @$);}
    |UNSIGNED VAR ';'{@$ = @1 + @3; $$ = std::make_unique<InitEmptyNode>($2, VarType::UNSIGNED, data, @$);}
    |SIGNED VAR';' {@$ = @1 + @3; $$ = std::make_unique<InitEmptyNode>($2, VarType::SIGNED, data, @$);}
    |CELL VAR ';' {@$ = @1 + @3; $$ = std::make_unique<InitEmptyNode>($2, VarType::CELL, data, @$);}
    |CONST UNSIGNED VAR ';'{@$ = @1 + @4; $$ = std::make_unique<InitEmptyNode>($3, VarType::UNSIGNED, data,@$,true);}
    |CONST SIGNED VAR';' {@$ = @1 + @4; $$ = std::make_unique<InitEmptyNode>($3, VarType::SIGNED, data,@$ ,true);}
    |CONST CELL VAR ';' {@$ = @1 + @3; $$ = std::make_unique<InitEmptyNode>($3, VarType::CELL, data,@$ ,true);}
    |MATRIX SIGNED VAR ';' {@$ = @1 + @3; $$ = std::make_unique<InitEmptyMatrixNode>($3, VarType::SIGNED, data, @$);}
    |MATRIX UNSIGNED VAR ';' {@$ = @1 + @3;$$ = std::make_unique<InitEmptyMatrixNode>($3, VarType::UNSIGNED, data, @$);}
    |MATRIX CELL VAR ';' {@$ = @1 + @3;$$ = std::make_unique<InitEmptyMatrixNode>($3, VarType::CELL, data, @$);}



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
        if($1[$3].second) error(@3, "already est takou noWall");
        $1[$3] = {false, true};
        $$ = std::move($1);
    }


    | cellArg ',' yesWall {
      if($1[$3].second) error(@3, "already est takou noWall");
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




%%

