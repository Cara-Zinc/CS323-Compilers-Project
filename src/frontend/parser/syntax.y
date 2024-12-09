%{
#include <stdio.h>
#include <stdlib.h>

#include "ast.h"            // 抽象语法树相关的函数和声明
#include "syntax.tab.h"
#include "syntax/syntax_analysis.h" // 语法分析相关的函数和声明
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
%token <str> INVALID TYPE STRUCT IF ELSE WHILE RETURN DOT SEMI COMMA ASSIGN LT LE GT GE NE EQ PLUS MINUS MUL DIV AND OR NOT LP RP LB RB LC RC STARTDEF ENDDEF

/* Declare non-terminals and their types */
%type <node> Program ExtDefList ExtDef ExtDecList Specifier StructSpecifier VarDec FunDec FunDef VarList ParamDec CompSt StmtList Stmt DefList Def DecList Dec Exp Args StructDefList StructDef


%start Program

%%

Program : ExtDefList { $$ = program_handler(pm, $1, yylineno); rprintAST($$, 0); }
        ;

ExtDefList : ExtDef ExtDefList { $$ = ext_def_list_handler(pm, $1, $2, yylineno); }
           | /* empty */ { $$ = createASTLeaf("ExtDefList", yylineno, NULL); }
           ;

ExtDef : Specifier ExtDecList SEMI { $$ = ext_def_dec_handler(pm, $1, $2, yylineno); }
       | Specifier SEMI { $$ = ext_def_struct_handler(pm, $1, yylineno); }
       | Specifier FunDec CompSt { $$ = ext_def_func_handler(pm, $1, $2, $3, yylineno); }
       | Specifier ExtDecList error { yyerror("Missing semicolon after declaration"); $$ = createASTLeaf("Error", yylineno, NULL); }
       | Specifier error { yyerror("Missing semicolon after declaration"); $$ = createASTLeaf("Error", yylineno, NULL); }
       | error ExtDecList SEMI { yyerror("Missing type specifier in declaration"); $$ = createASTLeaf("Error", yylineno, NULL); }
       ;

ExtDecList : VarDec { $$ = ext_dec_list_handler(pm, $1, NULL, yylineno); }
           | VarDec COMMA ExtDecList { $$ = ext_dec_list_handler(pm, $1, $3, yylineno); }
           | VarDec COMMA COMMA ExtDecList { yyerror("Extra comma in variable declaration"); $$ = createASTLeaf("Error", yylineno, NULL); }
           | VarDec COMMA error { yyerror("Missing variable after comma in variable declaration"); $$ = createASTLeaf("Error", yylineno, NULL); }
           ;

Specifier : TYPE { $$ = type_handler(pm, $1, yylineno); }
          | StructSpecifier { $$ = struct_spec_handler(pm, $1, yylineno); }
          ;

StructSpecifier : STRUCT ID LC StructDefList RC { $$ = struct_def_handler(pm, $2, $4, yylineno); }
                | STRUCT ID { $$ = struct_def_handler(pm, $2, NULL, yylineno); }
                | STRUCT error { yyerror("Missing struct name"); $$ = createASTLeaf("Error", yylineno, NULL); }
                ;

StructDefList : StructDef StructDefList { $$ = struct_member_list_handler(pm, $1, $2, yylineno); }
              | /* empty */ { $$ = struct_member_list_handler(pm, NULL, NULL, yylineno); }
              ;

StructDef : Specifier DecList SEMI { $$ = struct_member_handler(pm, $1, $2, NULL, yylineno); }
          | Specifier DecList error { yyerror("Missing semicolon after struct member declaration"); $$ = createASTLeaf("Error", yylineno, NULL); }
          | FunDef { $$ = $1; }
          ;

VarDec : ID { $$ = VarDec_ID_handler(pm, $1, yylineno); }
       | INVALID { $$ = createASTLeaf("Error", yylineno, NULL); }
       | VarDec LB INT RB { $$ = VarDec_Array_handler(pm, $1, $3, yylineno); }
       | VarDec LB error RB { yyerror("Invalid or missing array size"); $$ = createASTLeaf("Error", yylineno, NULL); }
       | VarDec LB RB { yyerror("Missing array size"); $$ = createASTLeaf("Error", yylineno, NULL); }
       | VarDec INT RB { yyerror("Missing left bracket in array declaration"); $$ = createASTLeaf("Error", yylineno, NULL); }
       | VarDec LB INT error { yyerror("Missing right bracket in array declaration"); $$ = createASTLeaf("Error", yylineno, NULL); }
       ;

FunDef : Specifier FunDec CompSt { $$ = struct_member_handler(pm, $1, $2, $3, yylineno); }
       ;

FunDec : ID LP VarList RP { $$ = FunDec_handler(pm, $1, $3, yylineno); }
       | ID LP RP { $$ = FunDec_handler(pm, $1, NULL, yylineno); }
       | ID RP { yyerror("Missing left parenthesis in function declaration"); $$ = createASTLeaf("Error", yylineno, NULL); }
       | ID LP error { yyerror("Missing closing parenthesis in function declaration. I"); $$ = createASTLeaf("Error", yylineno, NULL); }
       // | ID LP VarList SEMI { yyerror("Missing closing parenthesis in function declaration. II"); $$ = createASTLeaf("Error", yylineno, NULL); }
       | ID LP VarList error { yyerror("Missing closing parenthesis in function declaration. III"); $$ = createASTLeaf("Error", yylineno, NULL); }
       | ID { yyerror("Missing parameter list in function declaration"); $$ = createASTLeaf("Error", yylineno, NULL); }
       ;

VarList : ParamDec COMMA VarList { $$ = VarList_ParamDec_Comma_VarList_handler(pm, $1, $3, yylineno); }
        | ParamDec { $$ = VarList_ParamDec_handler(pm, $1, yylineno); }
        | error COMMA VarList { yyerror("Invalid or missing parameter declaration"); $$ = createASTLeaf("Error", yylineno, NULL); }
        | ParamDec COMMA error { yyerror("Extra comma in parameter list. Did you forget a parameter?"); $$ = createASTLeaf("Error", yylineno, NULL); }
        ;

ParamDec : Specifier VarDec { $$ = ParamDec_handler(pm, $1, $2, yylineno); }
         | Specifier error { yyerror("Missing variable declaration in parameter list"); $$ = createASTLeaf("Error", yylineno, NULL); }
         | error VarDec { yyerror("Missing type specifier in parameter list"); $$ = createASTLeaf("Error", yylineno, NULL); }
         | VarDec { yyerror("Missing type specifier in parameter list"); $$ = createASTLeaf("Error", yylineno, NULL); }
         ;

CompSt : LC STARTDEF DefList ENDDEF StmtList RC { $$ = compst_deflist_stmtlist_handler(pm, $3, $5, yylineno); }
       | LC StmtList RC { $$ = compst_deflist_stmtlist_handler(pm, NULL, $2, yylineno); }
       | error STARTDEF DefList ENDDEF StmtList RC { yyerror("Missing opening brace in compound statement"); $$ = createASTLeaf("Error", yylineno, NULL); }
       | LC STARTDEF DefList ENDDEF StmtList error { yyerror("Missing closing brace in compound statement"); $$ = createASTLeaf("Error", yylineno, NULL); }
       | LC DefList ENDDEF StmtList RC { yyerror("Missing STARTDEF in compound statement"); $$ = createASTLeaf("Error", yylineno, NULL); }
       ;

StmtList : { $$ = stmtlist_stmt_stmtlist_handler(pm, NULL, NULL, yylineno); } 
         | Stmt StmtList { $$ = stmtlist_stmt_stmtlist_handler(pm, $1, $2, yylineno); }
         | Stmt Def DefList StmtList { yyerror("Missing specifier"); $$ = createASTLeaf("Error", yylineno, NULL); }
         ;

Stmt : Exp SEMI { $$ = stmt_exp_handler(pm, $1, yylineno); }
     | CompSt { $$ = stmt_comp_handler(pm, $1, yylineno); }
     | RETURN Exp SEMI { $$ = stmt_return_handler(pm, $2, yylineno); }
     | IF LP Exp RP Stmt { $$ = stmt_if_handler(pm, $3, $5, yylineno); }
     | IF LP Exp RP Stmt ELSE Stmt { $$ = stmt_if_else_handler(pm, $3, $5, $7, yylineno); }
     | WHILE LP Exp RP Stmt { $$ = stmt_while_handler(pm, $3, $5, yylineno); }
     | Exp error { yyerror("Missing semicolon after expression"); $$ = createASTLeaf("Error", yylineno, NULL); }
     | RETURN Exp error { yyerror("Missing semicolon after return statement"); $$ = createASTLeaf("Error", yylineno, NULL); }
     | IF LP Exp error SEMI { yyerror("Missing closing parenthesis after if condition. Type I"); $$ = createASTLeaf("Error", yylineno, NULL); }
     | IF LP Exp error Stmt { yyerror("Missing closing parenthesis after if condition"); $$ = createASTLeaf("Error", yylineno, NULL); }
     | IF LP Exp RP error Stmt { yyerror("Missing statement after if condition"); $$ = createASTLeaf("Error", yylineno, NULL); }
     | IF LP error RP Stmt { yyerror("Invalid or missing expression in if condition"); $$ = createASTLeaf("Error", yylineno, NULL); }
     | IF LP Exp RP Stmt ELSE error { yyerror("Invalid or missing statement after else"); $$ = createASTLeaf("Error", yylineno, NULL); }
     | IF Exp RP Stmt { yyerror("Missing left parenthesis in if statement"); $$ = createASTLeaf("Error", yylineno, NULL); }
     | IF Exp RP Stmt ELSE Stmt { yyerror("Missing left parenthesis in if statement"); $$ = createASTLeaf("Error", yylineno, NULL); }
     | IF error { yyerror("Invalid if statement syntax"); $$ = createASTLeaf("Error", yylineno, NULL); }
     | ELSE error { yyerror("Invalid else statement syntax. Did you forget an if statement?"); $$ = createASTLeaf("Error", yylineno, NULL); }
     | WHILE LP Exp error SEMI { yyerror("Missing closing parenthesis after while condition. Type I"); $$ = createASTLeaf("Error", yylineno, NULL); }
     | WHILE LP Exp error Stmt { yyerror("Missing closing parenthesis after while condition"); $$ = createASTLeaf("Error", yylineno, NULL); }
     | WHILE LP error RP Stmt { yyerror("Invalid or missing expression in while condition"); $$ = createASTLeaf("Error", yylineno, NULL); }
     | WHILE Exp RP Stmt { yyerror("Missing left parenthesis in while statement"); $$ = createASTLeaf("Error", yylineno, NULL); }
     | WHILE error { yyerror("Invalid while statement syntax"); $$ = createASTLeaf("Error", yylineno, NULL); }
     // | LC DefList StmtList error { yyerror("Missing closing brace in compound statement"); $$ = createASTLeaf("Error", yylineno, NULL); }
     // | error DefList StmtList RC { yyerror("Missing opening brace in compound statement"); $$ = createASTLeaf("Error", yylineno, NULL); }
     // | LC DefList StmtList RC error { yyerror("Invalid or missing statement after compound statement"); $$ = createASTLeaf("Error", yylineno, NULL); }
     ;

DefList : Def DefList { $$ = deflist_def_deflist_handler(pm, $1, $2, yylineno); }
        | { $$ = deflist_def_deflist_handler(pm, NULL, NULL, yylineno); }
        ;

Def : Specifier DecList SEMI { $$ = def_specifier_declist_handler(pm, $1, $2, yylineno); }
    | Specifier DecList error { yyerror("Missing semicolon after declaration"); $$ = createASTLeaf("Error", yylineno, NULL); }
    // | DecList SEMI { yyerror("Missing type specifier in declaration"); $$ = createASTLeaf("Error", yylineno, NULL); }
    ;

DecList : Dec { $$ = DecList_handler(pm, $1, NULL, yylineno); }
        | Dec COMMA DecList { $$ = DecList_handler(pm, $1, $3, yylineno); }
        ;

Dec : VarDec { $$ = Dec_handler(pm, $1, NULL, yylineno); }
    | VarDec ASSIGN Exp { $$ = Dec_handler(pm, $1, $3, yylineno); }
    ;

Exp : Exp ASSIGN Exp { $$ = exp_assign_handler(pm, $1, $3, yylineno); }
    | Exp AND Exp { $$ = exp_bi_op_handler(pm, $1, $2, $3, yylineno); }
    | Exp OR Exp { $$ = exp_bi_op_handler(pm, $1, $2, $3, yylineno); }
    | Exp LT Exp { $$ = exp_bi_op_handler(pm, $1, $2, $3, yylineno); }
    | Exp LE Exp { $$ = exp_bi_op_handler(pm, $1, $2, $3, yylineno); }
    | Exp GT Exp { $$ = exp_bi_op_handler(pm, $1, $2, $3, yylineno); }
    | Exp GE Exp { $$ = exp_bi_op_handler(pm, $1, $2, $3, yylineno); }
    | Exp NE Exp { $$ = exp_bi_op_handler(pm, $1, $2, $3, yylineno); }
    | Exp EQ Exp { $$ = exp_bi_op_handler(pm, $1, $2, $3, yylineno); }
    | Exp PLUS Exp { $$ = exp_bi_op_handler(pm, $1, $2, $3, yylineno); }
    | Exp MINUS Exp { $$ = exp_bi_op_handler(pm, $1, $2, $3, yylineno); }
    | Exp MUL Exp { $$ = exp_bi_op_handler(pm, $1, $2, $3, yylineno); }
    | Exp DIV Exp { $$ = exp_bi_op_handler(pm, $1, $2, $3, yylineno); }
    | Exp INVALID Exp { $$ = createASTLeaf("Error", yylineno, NULL); }
    | LP Exp RP { $$ = $2; }
    | MINUS Exp { $$ = exp_unary_op_handler(pm, $1, $2, yylineno); }
    | PLUS Exp { $$ = exp_unary_op_handler(pm, $1, $2, yylineno); }
    | NOT Exp { $$ = exp_unary_op_handler(pm, $1, $2, yylineno); }
    | ID LP Args RP { $$ = exp_func_handler(pm, $1, $3, yylineno); }
    | ID LP RP { $$ = exp_func_handler(pm, $1, NULL, yylineno); }
    | Exp LB Exp RB { $$ = exp_array_handler(pm, $1, $3, yylineno); }
    | Exp DOT ID { $$ = exp_struct_handler(pm, $1, $3, yylineno); }
    | Exp DOT ID LP Args RP { $$ = exp_struct_func_handler(pm, $1, $3, $5, yylineno); }
    | Exp DOT ID LP RP { $$ = exp_struct_func_handler(pm, $1, $3, NULL, yylineno); }
    | ID { $$ = exp_id_handler(pm, $1, yylineno); }
    | INT { $$ = exp_int_handler(pm, $1, yylineno); }
    | FLOAT { $$ = exp_float_handler(pm, $1, yylineno); }
    | CHAR { $$ = exp_char_handler(pm, $1, yylineno); }
    | Exp ASSIGN error { yyerror("Invalid expression after '=' operator"); $$ = createASTLeaf("Error", yylineno, NULL); }
    | Exp AND error { yyerror("Invalid expression after '&&' operator"); $$ = createASTLeaf("Error", yylineno, NULL); }
    | Exp OR error { yyerror("Invalid expression after '||' operator"); $$ = createASTLeaf("Error", yylineno, NULL); }
    | Exp LT error { yyerror("Invalid expression after '<' operator"); $$ = createASTLeaf("Error", yylineno, NULL); }
    | Exp LE error { yyerror("Invalid expression after '<=' operator"); $$ = createASTLeaf("Error", yylineno, NULL); }
    | Exp GT error { yyerror("Invalid expression after '>' operator"); $$ = createASTLeaf("Error", yylineno, NULL); }
    | Exp GE error { yyerror("Invalid expression after '>=' operator"); $$ = createASTLeaf("Error", yylineno, NULL); }
    | Exp NE error { yyerror("Invalid expression after '!=' operator"); $$ = createASTLeaf("Error", yylineno, NULL); }
    | Exp EQ error { yyerror("Invalid expression after '==' operator"); $$ = createASTLeaf("Error", yylineno, NULL); }
    | Exp PLUS error { yyerror("Invalid expression after '+' operator"); $$ = createASTLeaf("Error", yylineno, NULL); }
    | Exp MINUS error { yyerror("Invalid expression after '-' operator"); $$ = createASTLeaf("Error", yylineno, NULL); }
    | Exp MUL error { yyerror("Invalid expression after '*' operator"); $$ = createASTLeaf("Error", yylineno, NULL); }
    | Exp DIV error { yyerror("Invalid expression after '/' operator"); $$ = createASTLeaf("Error", yylineno, NULL); }
    | MINUS error { yyerror("Invalid expression after unary operator '-'"); $$ = createASTLeaf("Error", yylineno, NULL); }
    | PLUS error { yyerror("Invalid expression after unary operator '+'"); $$ = createASTLeaf("Error", yylineno, NULL); }
    | NOT error { yyerror("Invalid expression after unary operator '!'"); $$ = createASTLeaf("Error", yylineno, NULL); }
    | ID LP error RP { yyerror("Invalid arguments in function call"); $$ = createASTLeaf("Error", yylineno, NULL); }
    | Exp LB error RB { yyerror("Invalid or missing array index"); $$ = createASTLeaf("Error", yylineno, NULL); }
    | Exp LB Exp error { yyerror("Missing right bracket in array index"); $$ = createASTLeaf("Error", yylineno, NULL); }
    | Exp DOT error { yyerror("Invalid member name after '.' operator"); $$ = createASTLeaf("Error", yylineno, NULL); }
    | LP Exp error { yyerror("Unbalanced parentheses: Missing closing parenthesis"); $$ = createASTLeaf("Error", yylineno, NULL); }
    | ID LP Args error { yyerror("Missing closing parenthesis in function call"); $$ = createASTLeaf("Error", yylineno, NULL); }
    | INVALID { $$ = createASTLeaf("Error", yylineno, NULL); }
    | error ASSIGN Exp { yyerror("Invalid expression before '=' operator"); $$ = createASTLeaf("Error", yylineno, NULL); }
    ;

Args : Exp COMMA Args {$$ = args_handler(pm, $1, $3, yylineno); }
     | Exp { $$ = args_handler(pm, $1, NULL, yylineno); }
     | error COMMA Args { yyerror("Invalid or missing argument in argument list. I"); $$ = createASTLeaf("Error", yylineno, NULL); }
     | Exp COMMA error { yyerror("Invalid or missing argument in argument list. II"); $$ = createASTLeaf("Error", yylineno, NULL); }
     | Exp COMMA { yyerror("Extra comma in argument list. Did you forget an argument?"); $$ = createASTLeaf("Error", yylineno, NULL); }
     ;

%%

int main() {
    pm = program_manager_new();
    return yyparse();
}