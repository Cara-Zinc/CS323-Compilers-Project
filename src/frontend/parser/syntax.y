%{
#include <stdio.h>
#include <stdlib.h>

#include "declarations.h"   // 定义 %union 和 %token
#include "program.h"        // 程序结构的辅助函数和声明
#include "type_struct.h"    // 类型和结构体相关的函数和声明
#include "declaration.h"    // 变量和函数声明相关的函数和声明
#include "compound_stmt.h"  // 复合语句相关的函数和声明
#include "stmt.h"           // 各种语句的函数和声明
#include "def.h"            // 定义相关的辅助函数和声明
#include "exp.h"            // 表达式相关的辅助函数和声明
#include "mm/program_mananger.h" // 程序管理器相关的函数和声明

program_mananger *pm = program_mananger_new();

void yyerror(const char *s) {
    fprintf(stderr, "Error: %s\n", s);
}
%}

%union {
    int int_val;
    float float_val;
    char char_val;
    char *str;
}

%token <str> ID
%token <int_val> INT
%token <float_val> FLOAT
%token <char_val> CHAR
%token TYPE STRUCT IF ELSE WHILE RETURN DOT SEMI COMMA ASSIGN LT LE GT GE NE EQ PLUS MINUS MUL DIV AND OR NOT LP RP LB RB LC RC

%start Program

%%

Program : ExtDefList {}
        ;

ExtDefList : ExtDef ExtDefList
           | /* empty */
           ;

ExtDef : Specifier ExtDecList SEMI 
       | Specifier SEMI
       | Specifier FunDec CompSt
       ;

ExtDecList : VarDec
           | VarDec COMMA ExtDecList
           ;

Specifier : TYPE 
          | StructSpecifier 
          ;

StructSpecifier : STRUCT ID LC DefList RC
                | STRUCT ID
                ;

VarDec : ID { $$ = VarDec_ID_handler(pm, $1); }
       | VarDec LB INT RB { $$ = VarDec_Array_handler(pm, $1, $3); }
       ;

FunDec : ID LP VarList RP
       | ID LP RP
       ;

VarList : ParamDec COMMA VarList
        | ParamDec
        ;

ParamDec : Specifier VarDec
         ;

CompSt : LC DefList StmtList RC
       ;

StmtList : Stmt StmtList
         | /* empty */
         ;

Stmt : Exp SEMI
     | CompSt
     | RETURN Exp SEMI
     | IF LP Exp RP Stmt
     | IF LP Exp RP Stmt ELSE Stmt
     | WHILE LP Exp RP Stmt
     ;

DefList : Def DefList
        | /* empty */
        ;

Def : Specifier DecList SEMI
    ;

DecList : Dec
        | Dec COMMA DecList
        ;

Dec : VarDec
    | VarDec ASSIGN Exp
    ;

Exp : Exp ASSIGN Exp { $$ = exp_assign_handler(pm, $1, $3); }
    | Exp AND Exp { $$ = exp_bi_op_handler(pm, $1, $2, $3); }
    | Exp OR Exp { $$ = exp_bi_op_handler(pm, $1, $2, $3); }
    | Exp LT Exp { $$ = exp_bi_op_handler(pm, $1, $2, $3); }
    | Exp LE Exp { $$ = exp_bi_op_handler(pm, $1, $2, $3); }
    | Exp GT Exp { $$ = exp_bi_op_handler(pm, $1, $2, $3); }
    | Exp GE Exp { $$ = exp_bi_op_handler(pm, $1, $2, $3); }
    | Exp NE Exp { $$ = exp_bi_op_handler(pm, $1, $2, $3); }
    | Exp EQ Exp { $$ = exp_bi_op_handler(pm, $1, $2, $3); }
    | Exp PLUS Exp { $$ = exp_bi_op_handler(pm, $1, $2, $3); }
    | Exp MINUS Exp { $$ = exp_bi_op_handler(pm, $1, $2, $3); }
    | Exp MUL Exp { $$ = exp_bi_op_handler(pm, $1, $2, $3); }
    | Exp DIV Exp { $$ = exp_bi_op_handler(pm, $1, $2, $3); }
    | LP Exp RP { $$ = $2 }
    | MINUS Exp { $$ = exp_unary_op_handler(pm, $1, $2); }
    | NOT Exp { $$ = exp_unary_op_handler(pm, $1, $2); }
    | ID LP Args RP { /* funtion call }*/ }
    | ID LP RP { /* funtion call }*/ }
    | Exp LB Exp RB { array_handler(pm, $1, $3); }
    | Exp DOT ID { struct_member_handler(pm, $1, $3); }
    | ID { exp_id_handler(pm, $1); }
    | INT { $$ = exp_primitive_handler(pm, "INT", $1); }
    | FLOAT { $$ = exp_primitive_handler(pm, "FLOAT", $1); }
    | CHAR { $$ = exp_primitive_handler(pm, "CHAR", $1); }
    ;

Args : Exp COMMA Args
     | Exp
     ;

%%

int main() {
    return yyparse();
}