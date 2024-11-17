%{
#include <stdio.h>
#include <stdlib.h>

#include "declarations.h"   // 定义 %union 和 %token
#include "program.h"        // 程序结构的辅助函数和声明
#include "type_struct.h"    // 类型和结构体相关的函数和声明
#include "declaration.h"    // 变量和函数声明相关的函数和声明
#include "compst.h"         // 复合语句相关的函数和声明
#include "stmt.h"           // 各种语句的函数和声明
#include "def.h"            // 定义相关的辅助函数和声明
#include "exp.h"            // 表达式相关的辅助函数和声明
#include "ext.h"            
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

Program : ExtDefList { $$ = $1; }
        ;

ExtDefList : ExtDef ExtDefList { $$ = ext_def_list_handler(pm, $1, $2); }
           | /* empty */
           ;

ExtDef : Specifier ExtDecList SEMI { $$ = ext_def_dec_handler(pm, $1, $2); }
       | Specifier SEMI { $$ = ext_def_struct_handler(pm, $1); }
       | Specifier FunDec CompSt { $$ = ext_def_func_handler(pm, $1, $2, $3); }
       ;

ExtDecList : VarDec { $$ = ext_dec_list_handler(pm, $1, NULL); }
           | VarDec COMMA ExtDecList { $$ = ext_dec_list_handler(pm, $1, $3); }
           ;

Specifier : TYPE { $$ = type_handler(pm, $1); }
          | StructSpecifier { $$ = struct_spec_handler(pm, $1); }
          ;

StructSpecifier : STRUCT ID LC DefList RC { $$ = struct_def_handler(pm, $2, $4); }
                | STRUCT ID { $$ = struct_dec_handler(pm, $2, NULL); }
                ;

VarDec : ID { $$ = VarDec_ID_handler(pm, $1); }
       | VarDec LB INT RB { $$ = VarDec_Array_handler(pm, $1, $3); }
       ;

FunDec : ID LP VarList RP { $$ = FunDec_handler(pm, $1, $3); }
       | ID LP RP { $$ = FunDec_handler(pm, $1, NULL); }
       ;

VarList : ParamDec COMMA VarList { $$ = VarList_ParamDec_Comma_VarList_handler(pm, $1, $3); }
        | ParamDec {$$ = VarList_ParamDec_handler(pm, $1); }
        ;

ParamDec : Specifier VarDec { $$ = ParamDec_handler(pm, $1, $2); }
         ;

CompSt : LC DefList StmtList RC { $$ = compst_deflist_stmtlist_handler(pm, $2, $3); }
       ;

StmtList : Stmt StmtList { $$ = stmtlist_stmt_stmtlist_handler(pm, $1, $2); }
         | Stmt { $$ = stmtlist_stmt_handler(pm, $1); }
         ;

Stmt : Exp SEMI { $$ = stmt_exp_handler(pm, $1); }
     | CompSt { $$ = stmt_compst_handler(pm, $1); }
     | RETURN Exp SEMI { $$ = stmt_return_handler(pm, $2); }
     | ELSE Stmt { $$ = stmt_else_handler(pm, $3, $5, $7); }
     | IF LP Exp RP Stmt { $$ = stmt_if_handler(pm, $3, $5); }
     | WHILE LP Exp RP Stmt { $$ = stmt_while_handler(pm, $3, $5); }
     ;

DefList : Def DefList { $$ = deflist_def_deflist_handler(pm, $1, $2); }
        | Def { $$ = deflist_def_handler(pm, $1); }
        ;

Def : Specifier DecList SEMI { $$ = def_specifier_declist_handler(pm, $1, $2)}
    ;

DecList : Dec { $$ = DecList_handler(pm, $1, NULL); }
        | Dec COMMA DecList { $$ = DecList_handler(pm, $1, $3); }
        ;

Dec : VarDec { $$ = Dec_handler(pm, $1, NULL); }
    | VarDec ASSIGN Exp { $$ = Dec_handler(pm, $1, $3); }
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

Args : Exp COMMA Args {$$ = args_handler(pm, $1, $3); }
     | Exp { $$ = args_handler(pm, $1, NULL); }
     ;

%%

int main() {
    return yyparse();
}