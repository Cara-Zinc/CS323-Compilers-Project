%{
#include "syntax.tab.h"
%}

%%
[ \t\n]+          ;                    // ignore whitespaces

0|[1-9][0-9]*           { yylval = atoi(yytext); return INT; }
0x[0-9a-fA-F]+          { yylval = strtol(yytext, NULL, 16); return INT; }
[1-9][0-9]*\.[0-9]+     { yylval = atof(yytext); return FLOAT; }
0\.[0-9]+               { yylval = atof(yytext); return FLOAT; }
\'[ -~]\'               { yylval = yytext[1]; return CHAR; }

\'\\x[0-9a-fA-F]{2}\' { 
    yylval = (char)strtol(&yytext[2], NULL, 16); 
    return CHAR; 
}
\'\\x.*\' {
    printf("Error: illegal hex char '%s'\n", yytext);
    return -1;
}
[a-zA-Z_][a-zA-Z0-9_]*  { return ID; }
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
.                       { return yytext[0]; }
%%