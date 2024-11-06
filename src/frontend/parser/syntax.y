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

Program : ExtDefList
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

VarDec : ID
       | VarDec LB INT RB
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

Exp : Exp ASSIGN Exp
    | Exp AND Exp 
    | Exp OR Exp 
    | Exp LT Exp
    | Exp LE Exp
    | Exp GT Exp
    | Exp GE Exp
    | Exp NE Exp
    | Exp EQ Exp
    | Exp PLUS Exp 
    | Exp MINUS Exp
    | Exp MUL Exp
    | Exp DIV Exp
    | LP Exp RP
    | MINUS Exp
    | NOT Exp
    | ID LP Args RP
    | ID LP RP
    | Exp LB Exp RB
    | Exp DOT ID
    | ID
    | INT
    | FLOAT
    | CHAR
    ;

Args : Exp COMMA Args
     | Exp
     ;

%%

int main() {
    return yyparse();
}