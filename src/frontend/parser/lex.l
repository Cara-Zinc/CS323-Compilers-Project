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
%}

%option yylineno

%%
[ \t\n]+          ;                    // ignore whitespaces

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
[a-zA-Z_][a-zA-Z0-9_]*  { yylval.str = strdup(yytext); return ID; }
"if"                    { return IF; }
"else"                  { return ELSE; }
"while"                 { return WHILE; } 
"return"                { return RETURN; }
"struct"                { return STRUCT; }
"int"                   { return TYPE; }
"float"                 { return TYPE; }
"char"                  { return TYPE; }
"=="                    { return EQ; }
"="                     { return ASSIGN; }
"<"                     { return LT; }
"<="                    { return LE; }
">"                     { return GT; }
">="                    { return GE; }
"!="                    { return NE; }
"+"                     { return PLUS; }
"-"                     { return MINUS; }
"*"                     { return MUL; }
"/"                     { return DIV; }
"&&"                    { return AND; }
"||"                    { return OR; }
"!"                     { return NOT; }
"("                     { return LP; }
")"                     { return RP; }
"["                     { return LB; }
"]"                     { return RB; }
"{"                     { return LC; }
"}"                     { return RC; }
";"                     { return SEMI; }
","                     { return COMMA; }
"."                     { return DOT; }

.+                      { yyerrora(yytext); return -1; }
%%