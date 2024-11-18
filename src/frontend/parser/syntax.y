%{
#include <stdio.h>
#include <stdlib.h>

#include "ast.h"            // 抽象语法树相关的函数和声明
#include "syntax.tab.h"
#include "args.h"           // 函数参数相关的函数和声明
//#include "program.h"        // 程序结构的辅助函数和声明
//#include "type_struct.h"    // 类型和结构体相关的函数和声明
#include "declaration.h"    // 变量和函数声明相关的函数和声明
#include "compst.h"         // 复合语句相关的函数和声明
#include "stmt.h"           // 各种语句的函数和声明
#include "def.h"            // 定义相关的辅助函数和声明
#include "exp.h"            // 表达式相关的辅助函数和声明
#include "ext.h"
#include "spec.h"           // 基础类型和结构体相关的辅助函数和声明
#include "../mm/program_manager.h" // 程序管理器相关的函数和声明


int yylex(void);
extern int yylineno;
program_manager *pm;

void yyerror(const char *s) {
    fprintf(stderr, "Error type B at Line %d: %s\n", yylineno, s);
}
%}

%union {
    int int_val;
    float float_val;
    char char_val;
    char *str;
    ASTNode *node;
}

%token <str> ID
%token <int_val> INT
%token <float_val> FLOAT
%token <char_val> CHAR
%token TYPE STRUCT IF ELSE WHILE RETURN DOT SEMI COMMA ASSIGN LT LE GT GE NE EQ PLUS MINUS MUL DIV AND OR NOT LP RP LB RB LC RC

/* Declare non-terminals and their types */
%type <node> Program ExtDefList ExtDef ExtDecList Specifier StructSpecifier VarDec FunDec VarList ParamDec CompSt StmtList Stmt DefList Def DecList Dec Exp Args
%type <str> TYPE STRUCT PLUS MINUS MUL DIV AND OR NOT LT LE GT GE NE EQ

%start Program

%%

Program : ExtDefList { $$ = program_handler(pm, $1); rprintAST($$, 0); }
        ;

ExtDefList : ExtDef ExtDefList { $$ = ext_def_list_handler(pm, $1, $2); }
           | /* empty */ { $$ = createASTLeaf("ExtDefList", NULL); }
           ;

ExtDef : Specifier ExtDecList SEMI { $$ = ext_def_dec_handler(pm, $1, $2); }
       | Specifier ExtDecList error { yyerror("Missing semicolon ';'"); }
       | ExtDecList SEMI error { yyerror("Missing specifier"); }
       | Specifier SEMI { $$ = ext_def_struct_handler(pm, $1); }
       | Specifier error { yyerror("Missing semicolon ';'"); }
       | Specifier FunDec CompSt { $$ = ext_def_func_handler(pm, $1, $2, $3); }
       ;

ExtDecList : VarDec { $$ = ext_dec_list_handler(pm, $1, NULL); }
           | VarDec COMMA ExtDecList { $$ = ext_dec_list_handler(pm, $1, $3); }
           | VarDec COMMA error { yyerror("Missing variable name"); }
           ;

Specifier : TYPE { $$ = type_handler(pm, $1); }
          | StructSpecifier { $$ = struct_spec_handler(pm, $1); }
          ;

StructSpecifier : STRUCT ID LC DefList RC { $$ = struct_def_handler(pm, $2, $4); }
                | STRUCT ID { $$ = struct_def_handler(pm, $2, NULL); }
                ;

VarDec : ID { $$ = VarDec_ID_handler(pm, $1); }
       | VarDec LB INT RB { $$ = VarDec_Array_handler(pm, $1, $3); }
       | VarDec LB INT error { yyerror("Missing closing bracket ']'"); }
       ;

FunDec : ID LP VarList RP { $$ = FunDec_handler(pm, $1, $3); }
       | ID LP VarList error { yyerror("Missing closing parenthesis ')'"); }
       | ID LP RP { $$ = FunDec_handler(pm, $1, NULL); }
       | ID LP error { yyerror("Missing closing parenthesis ')'"); }
       ;

VarList : ParamDec COMMA VarList { $$ = VarList_ParamDec_Comma_VarList_handler(pm, $1, $3); }
        | ParamDec {$$ = VarList_ParamDec_handler(pm, $1); }
        | ParamDec COMMA COMMA VarList error { yyerror("Missing variable"); }
        | ParamDec COMMA error { yyerror("Missing variable"); }
        ;

ParamDec : Specifier VarDec { $$ = ParamDec_handler(pm, $1, $2); }
         | VarDec error { yyerror("Missing specifier"); }
         | Specifier error { yyerror("Missing variable name"); }
         ;

CompSt : LC DefList StmtList RC { $$ = compst_deflist_stmtlist_handler(pm, $2, $3); }
       ;

StmtList : Stmt StmtList { $$ = stmtlist_stmt_stmtlist_handler(pm, $1, $2); }
         | Stmt { $$ = stmtlist_stmt_handler(pm, $1); }
         ;

Stmt : Exp SEMI { $$ = stmt_exp_handler(pm, $1); }
     | CompSt { $$ = stmt_comp_handler(pm, $1); }
     | RETURN Exp SEMI { $$ = stmt_return_handler(pm, $2); }
     | RETURN Exp error { yyerror("Missing semicolon ';'"); }
     | IF LP Exp RP Stmt { $$ = stmt_if_handler(pm, $3, $5); }
     | IF LP RP Stmt error { yyerror("Missing Exp in if statement"); }
     | IF Exp RP Stmt error { yyerror("Missing opening parenthesis '('"); }
     | IF LP Exp Stmt error { yyerror("Missing closing parenthesis ')'"); }
     | IF LP Exp RP Stmt ELSE Stmt { $$ = stmt_if_else_handler(pm, $3, $5, $7); }
     | IF LP RP Stmt ELSE Stmt error { yyerror("Missing Exp in if statement"); }
     | IF Exp RP Stmt ELSE Stmt error { yyerror("Missing opening parenthesis '('"); }
     | IF LP Exp Stmt ELSE Stmt error { yyerror("Missing closing parenthesis ')'"); }
     | ELSE Stmt error { yyerror("Missing if statement"); }
     | WHILE LP Exp RP Stmt { $$ = stmt_while_handler(pm, $3, $5); }
     | WHILE LP Exp Stmt error { yyerror("Missing closing parenthesis ')'"); }
     | WHILE Exp RP Stmt error { yyerror("Missing opening parenthesis '('"); }
     ;

DefList : Def DefList { $$ = deflist_def_deflist_handler(pm, $1, $2); }
        | Def { $$ = deflist_def_handler(pm, $1); }
        ;

Def : Specifier DecList SEMI { $$ = def_specifier_declist_handler(pm, $1, $2)}
    | Specifier DecList error { yyerror("Missing semicolon ';'"); }
    | DecList SEMI error { yyerror("Missing specifier"); }
    ;

DecList : Dec { $$ = DecList_handler(pm, $1, NULL); }
        | Dec COMMA DecList { $$ = DecList_handler(pm, $1, $3); }
        ;

Dec : VarDec { $$ = Dec_handler(pm, $1, NULL); }
    | VarDec ASSIGN Exp { $$ = Dec_handler(pm, $1, $3); }
    ;

Exp : Exp ASSIGN Exp { $$ = exp_assign_handler(pm, $1, $3); }
    | Exp ASSIGN error { yyerror("Missing right value after ="); }
    | ASSIGN Exp error { yyerror("Missing left value before ="); }
    | Exp AND Exp { $$ = exp_bi_op_handler(pm, $1, $2, $3); }
    | Exp OR Exp { $$ = exp_bi_op_handler(pm, $1, $2, $3); }
    | Exp LT Exp { $$ = exp_bi_op_handler(pm, $1, $2, $3); }
    | Exp LE Exp { $$ = exp_bi_op_handler(pm, $1, $2, $3); }
    | Exp GT Exp { $$ = exp_bi_op_handler(pm, $1, $2, $3); }
    | Exp GE Exp { $$ = exp_bi_op_handler(pm, $1, $2, $3); }
    | Exp NE Exp { $$ = exp_bi_op_handler(pm, $1, $2, $3); }
    | Exp EQ Exp { $$ = exp_bi_op_handler(pm, $1, $2, $3); }
    | Exp PLUS Exp { $$ = exp_bi_op_handler(pm, $1, $2, $3); }
    | Exp PLUS error { yyerror("Missing Exp after +"); }
    | Exp MINUS Exp { $$ = exp_bi_op_handler(pm, $1, $2, $3); }
    | Exp MUL Exp { $$ = exp_bi_op_handler(pm, $1, $2, $3); }
    | Exp MUL error { yyerror("Missing Exp after *"); }
    | Exp DIV Exp { $$ = exp_bi_op_handler(pm, $1, $2, $3); }
    | Exp DIV error { yyerror("Missing Exp after /"); }
    | LP Exp RP { $$ = $2 }
    | LP Exp error { yyerror("Missing closing parenthesis ')'"); }
    | MINUS Exp { $$ = exp_unary_op_handler(pm, $1, $2); }
    | PLUS Exp { $$ = exp_unary_op_handler(pm, $1, $2); }
    | NOT Exp { $$ = exp_unary_op_handler(pm, $1, $2); }
    | ID LP Args RP { exp_func_handler(pm, $1, $3); }
    | ID LP RP { exp_func_handler(pm, $1, NULL); }
    | ID LP Args error { yyerror("Missing closing parenthesis ')'"); }
    | ID LP error { yyerror("Missing closing parenthesis ')'"); }
    | Exp LB Exp RB { exp_array_handler(pm, $1, $3); }
    | Exp LB Exp error { yyerror("Missing closing bracket ']'"); }
    | Exp DOT ID { exp_struct_handler(pm, $1, $3); }
    | Exp DOT error { yyerror("Missing struct member"); }
    | ID { exp_id_handler(pm, $1); }
    | INT { $$ = exp_int_handler(pm, $1); }
    | FLOAT { $$ = exp_float_handler(pm, $1); }
    | CHAR { $$ = exp_char_handler(pm, $1); }
    ;

Args : Exp COMMA Args {$$ = args_handler(pm, $1, $3); }
     | Exp COMMA error { yyerror("Missing argument"); }
     | Exp { $$ = args_handler(pm, $1, NULL); }
     ;

%%

int main() {
    pm = program_manager_new();
    return yyparse();
}