%{
#include "ast.h"
#include "syntax.tab.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
void yyerrora(const char *s) {
    fprintf(stderr, "Error type A at Line %d: unknown lexeme %s\n", yylineno, s);
}

#include "../utils/copy.h"
%}

%option yylineno

%%
[ \t\n]+          {}                    // ignore whitespaces
0|[1-9][0-9]*           { yylval.int_val = atoi(yytext); return INT; }
0x0|0x[1-9a-zA-Z][0-9a-fA-F]*          { yylval.int_val = strtol(yytext, NULL, 16); return INT; }
[1-9][0-9]*\.[0-9]+     { yylval.float_val = atof(yytext); return FLOAT; }
0\.[0-9]+               { yylval.float_val = atof(yytext); return FLOAT; }
\'[ -~]\'               { yylval.char_val = yytext[1]; return CHAR; }
\'\\x[0-9a-fA-F]{2}\' { 
    yylval.char_val = (char)strtol(&yytext[2], NULL, 16); 
    return CHAR; 
}
\'\\x.*\' {
    printf("Error: illegal hex char '%s'\n", yytext);
    return -1;
}

"if"                    { yylval.str = "IF"; return IF; }
"else"                  { yylval.str = "ELSE"; return ELSE; }
"while"                 { yylval.str = "WHILE"; return WHILE; } 
"return"                { yylval.str = "RETURN"; return RETURN; }
"struct"                { yylval.str = "STRUCT"; return STRUCT; }
"int"                   { yylval.str = "int"; return TYPE; }
"float"                 { yylval.str = "float"; return TYPE; }
"char"                  { yylval.str = "char"; return TYPE; }
[a-zA-Z_][a-zA-Z0-9_]*  { yylval.str = str_copy(yytext); return ID; }
"=="                    { yylval.str = "EQ"; return EQ; }
"="                     { yylval.str = "ASSIGN"; return ASSIGN; }
"<"                     { yylval.str = "LT"; return LT; }
"<="                    { yylval.str = "LE"; return LE; }
">"                     { yylval.str = "GT"; return GT; }
">="                    { yylval.str = "GE"; return GE; }
"!="                    { yylval.str = "NE"; return NE; }
"+"                     { yylval.str = "PLUS"; return PLUS; }
"-"                     { yylval.str = "MINUS"; return MINUS; }
"*"                     { yylval.str = "MUL"; return MUL; }
"/"                     { yylval.str = "DIV"; return DIV; }
"&&"                    { yylval.str = "AND"; return AND; }
"||"                    { yylval.str = "OR"; return OR; }
"!"                     { yylval.str = "NOT"; return NOT; }
"("                     { yylval.str = "LP"; return LP; }
")"                     { yylval.str = "RP"; return RP; }
"["                     { yylval.str = "LB"; return LB; }
"]"                     { yylval.str = "RB"; return RB; }
"{"                     { yylval.str = "LC"; return LC; }
"}"                     { yylval.str = "RC"; return RC; }
";"                     { yylval.str = "SEMI"; return SEMI; }
","                     { yylval.str = "COMMA"; return COMMA; }
"."                     { yylval.str = "DOT"; return DOT; }

.                      { yyerrora(yytext); return -1; }
%%