%{
#include "ast.h"
#include "syntax.tab.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

int lineno = 1;
void yyerrora(const char *s) {
    fprintf(stderr, "Error type A at Line %d: unknown lexeme %s\n", lineno, s);
}

#include "../utils/copy.h"
%}

%option yylineno

%%
\n                      { lineno++; }
[ \t]+                  {}                    // ignore whitespaces
0[0-9]+                 { yyerrora(yytext); yylval.str = str_copy(yytext); return INVALID; }
0[0-9]+\.[0-9]+         { yyerrora(yytext); yylval.str = str_copy(yytext); return INVALID; }
0|[1-9][0-9]*           { yylval.int_val = atoi(yytext); return INT; }
0[xX][0-9a-zA-Z]+       {
    // check if the hex number match regex 0[xX][0-9a-fA-F]+
    if (strspn(yytext, "0123456789abcdefABCDEFxX") == strlen(yytext)) {
        yylval.int_val = strtol(yytext, NULL, 16);
        return INT;
    } else {
        yyerrora(yytext);
        yylval.str = str_copy(yytext);
        return INVALID;  
    }
}
[0-9][0-9_a-zA-Z]+            { yyerrora(yytext); yylval.str = str_copy(yytext); return INVALID; }
[1-9][0-9]*\.[0-9]+     { yylval.float_val = atof(yytext); return FLOAT; }
0\.[0-9]+               { yylval.float_val = atof(yytext); return FLOAT; }
\'[ -~]\'               { yylval.char_val = yytext[1]; return CHAR; }
\'\\x[0-9a-zA-Z]+\' { 
    if (strlen(yytext) != 6) {
        yyerrora(yytext);
        yylval.str = str_copy(yytext);
        return INVALID;  // Return INVALID for bad hex char
    }
    char str[3];
    str[0] = yytext[3];
    str[1] = yytext[4];
    str[2] = '\0';
    if (strspn(str, "0123456789abcdefABCDEF") != 2) {
        yyerrora(yytext);
        yylval.str = str_copy(yytext);
        return INVALID;  // Return INVALID for bad hex char
    }
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
"void"                  { yylval.str = "void"; return TYPE; }
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

.                      { yyerrora(yytext); return INVALID; }
%%